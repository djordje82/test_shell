/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 13:41:44 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function restores the standard file descriptors to their original values.*/
void	restore_std_fds(int stdin_fd, int stdout_fd)
{
	if (dup2(stdin_fd, STDIN_FILENO) == -1)
		perror("dup2 stdin");
	if (dup2(stdout_fd, STDOUT_FILENO) == -1)
		perror("dup2 stdout");
	close(stdin_fd);
	close(stdout_fd);
}

int	create_process(pid_t *pid, t_shell *shell)
{
	*pid = fork();
	if (*pid == -1)
		return (cleanup_and_exit("fork failed", NULL, 1, shell));
	return (1);
}

int	is_parent_only_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0);
}

/*This function initializes a pipeline. It sets up pipe file descriptors and checks for parent-only builtins.*/
static int	init_pipeline(t_command *current, int *pipe_fd, t_shell *shell)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && !create_pipe(pipe_fd, shell))
		return (0);
	if (current->args && is_parent_only_builtin(current->args[0]))
		handle_builtin_cmd(current, shell);
	return (1);
}

/*This function sets up pipeline steps. It initializes pipe file descriptors,
	checks for parent-only builtins, and prepares for process creation.*/
int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!current->is_valid)
	{
		if (!current->prev)
		{
			print_command_not_found(current);
			return (0);
		}
	}
	signal(SIGINT, SIG_IGN);
	if (!init_pipeline(current, pipe_fd, shell))
		return (0);
	if (!create_process(&pid, shell))
	{
		close_pipe_ends(prev_pipe);
		close_pipe_ends(pipe_fd);
		return (0);
	}
	if (pid == 0)
	{
		if (!current->is_valid)
		{
			close_pipe_ends(prev_pipe);
			close_pipe_ends(pipe_fd);
			exit(127);
		}
		handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
	}
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
}

/*This function sets up and executes a command in a pipeline. It handles signals,
	redirections, and execution errors.*/
void	execute_pipeline_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
		exit(1);
	execve(cmd_path, cmd->args, shell->envp);
	handle_command_errors(cmd_path, cmd->args[0]);
}