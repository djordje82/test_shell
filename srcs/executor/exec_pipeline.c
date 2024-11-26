/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/26 11:07:14 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe)
	{
		if (prev_pipe[0] != -1)
			close(prev_pipe[0]);
		if (prev_pipe[1] != -1)
			close(prev_pipe[1]);
		prev_pipe[0] = prev_pipe[1] = -1;
	}
	if (pipe_fd)
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		pipe_fd[0] = pipe_fd[1] = -1;
	}
}

void	setup_pipe_redirections(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe && prev_pipe[0] != -1)
	{
		if (dup2(prev_pipe[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed IN");
			exit(1);
		}
		close(prev_pipe[0]);
	}
	if (pipe_fd && pipe_fd[1] != -1)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed OUT");
			exit(1);
		}
		close(pipe_fd[1]);
	}
}

static int	init_pipeline(t_command *current, int *pipe_fd, t_shell *shell)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && pipe(pipe_fd) == -1)
	{
		close_pipe(NULL, pipe_fd);
		return (cleanup_and_exit("pipe failed", NULL, 1, shell));
	}
	if (current->args && is_parent_only_builtin(current->args[0]))
		handle_builtin_cmd(current, shell);
	return (1);
}

int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int					pipe_fd[2];
	pid_t				pid;
	struct sigaction	sa_old_int;
	struct sigaction	sa;

	if (sigaction(SIGINT, NULL, &sa_old_int) == -1)
	{
		perror("sigaction");
		return (0);
	}
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	if (!current->is_valid)
	{
		if (!current->prev) // If it's first command
		{
			print_command_not_found(current);
			sigaction(SIGINT, &sa_old_int, NULL);
			return (0); // Stop entire pipeline
		}
	}
	if (!init_pipeline(current, pipe_fd, shell))
	{
		sigaction(SIGINT, &sa_old_int, NULL);
		return (0);
	}
	if (!create_process(&pid, shell))
	{
		close_pipe(prev_pipe, pipe_fd);
		sigaction(SIGINT, &sa_old_int, NULL);
		return (0);
	}
	if (pid == 0)
	{
		init_signals(false);
		if (!current->is_valid)
		{
			printf("minishell: %s: command not found\n", current->args[0]); //bebug
			close_pipe(prev_pipe, pipe_fd);
			exit(127);
		}
		handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
	}
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	sigaction(SIGINT, &sa_old_int, NULL);
	return (1);
}

/*This function sets up and executes a command in a pipeline. It handles signals,
	redirections, and execution errors.*/
void	execute_pipeline_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	init_signals(false);
	if (!setup_redirections(cmd))
		exit(1);
	execve(cmd_path, cmd->args, shell->envp);
	handle_command_errors(cmd_path, cmd->args[0]);
	exit(127);
}
