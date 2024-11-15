/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:13 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:16 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_parent_only_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0);
}

static int	handle_single_builtin(t_command *cmd, t_shell *shell)
{
	int	status;
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd))
		return (0);
	status = execute_builtin(cmd, shell);
	reset_redirections(stdin_fd, stdout_fd);
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
	{
		shell->running = false;
		shell->exit_status = status;
	}
	return (status);
}

void	handle_child_process(t_command *cmd, int *prev_pipe, int *pipe_fd,
		t_shell *shell)
{
	int	status;

	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	setup_child_signals(); // Use your existing function
	status = execute_single_command(cmd, shell);
	exit(status); // Always exit with the command's status
}

void	handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (pipe_fd[1] != -1)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
}

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	wpid;

	while ((wpid = wait(&status)) > 0)
	{
		if (WIFSIGNALED(status))
		{
			g_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 18);
			else if (WTERMSIG(status) == SIGINT)
				write(STDERR_FILENO, "\n", 1);
		}
		else if (WIFEXITED(status))
		{
			if (wpid == last_pid)
				g_exit_status = WEXITSTATUS(status);
		}
	}
}

int	execute_single_command(t_command *cmd, t_shell *shell)
{
	int	stdin_fd;
	int	stdout_fd;
	int	status;
	int	i;

	// Skip if no arguments at all
	if (!cmd->args)
		return (0);
	// If first arg is empty but there are more args, shift them left
	if (cmd->args[0] && !cmd->args[0][0])
	{
		i = 0;
		while (cmd->args[i + 1])
		{
			cmd->args[i] = cmd->args[i + 1];
			i++;
		}
		cmd->args[i] = NULL;
	}
	// Now check if we have any arguments left
	if (!cmd->args[0])
		return (0);
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd))
		return (0);
	// printf("Debug: Checking if builtin: %s\n", cmd->args[0]);
	if (is_builtin(cmd->args[0]))
	{
		// printf("Debug: Executing builtin command\n");
		status = execute_builtin(cmd, shell);
		// Only exit if it's the exit command AND the status indicates we should exit
		if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
		{
			reset_redirections(stdin_fd, stdout_fd);
			exit(status);
		}
	}
	else
	{
		// printf("Debug: Executing external command\n");
		status = execute_external(cmd, shell);
	}
	reset_redirections(stdin_fd, stdout_fd);
	return (status);
}

int	execute_commands(t_shell *shell)
{
	t_command			*current;
	int					prev_pipe[2];
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;
	pid_t				last_pid;

	current = shell->cmnd_lst;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	last_pid = 0;
	setup_execution_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
		return (handle_single_builtin(current, shell));
	while (current)
	{
		if (!handle_pipeline(current, prev_pipe, &last_pid, shell))
			return (1);
		current = current->next;
	}
	wait_for_children(last_pid);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (g_exit_status);
}
