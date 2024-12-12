/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/12 17:43:31 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipeline(t_command *current, int *pipe_fd, t_shell *shell)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && !create_pipe(pipe_fd, shell))
	{
		perror("pipe failed");
		return (0);
	}
	if (current->args && is_builtin(current->args[0]))
	{
		if (is_parent_only_builtin(current->args[0]) && !current->next
			&& !current->prev)
		{
			if (!setup_redirections(current))
				return (0);
			handle_builtin_cmd(current, shell);
			return (1);
		}
	}
	return (1);
}

static int	handle_invalid_command(t_command *current)
{
	if (!current->is_valid)
	{
		if (!current->prev)
			print_command_not_found(current);
		return (0);
	}
	return (1);
}

static void	handle_child_process(t_command *current, int *prev_pipe,
		int *pipe_fd, t_shell *shell)
{
	setup_child_signal();
	if (!current->is_valid)
	{
		cleanup_pipeline_resources(prev_pipe, pipe_fd);
		exit(127);
	}
	handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
	
}

// New function in exec_pipeline.c or a new file heredoc_pipeline.c
static int handle_pipeline_heredocs(t_command *command_list)
{
    t_command *current;
    pid_t heredoc_pid;
    int status;

    current = command_list;
    while (current)
    {
        if (current->in_type == REDIR_HEREDOC)
        {
            heredoc_pid = fork();
            if (heredoc_pid == -1)
                return (0);
            
            if (heredoc_pid == 0)
            {
                setup_heredoc_signals();
                if (!setup_heredoc(current))
                    exit(1);
                exit(0);
            }
            
            // Parent waits for each heredoc to complete
            waitpid(heredoc_pid, &status, 0);
            if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
                return (0);
        }
        current = current->next;
    }
    return (1);
}

/* int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (!handle_invalid_command(current))
		return (0);
	signal(SIGINT, SIG_IGN);
	if (!init_pipeline(current, pipe_fd, shell))
		return (0);
	if (!create_process(&pid, shell))
	{
		cleanup_pipeline_resources(prev_pipe, pipe_fd);
		return (0);
	}
	if (pid == 0)
		handle_child_process(current, prev_pipe, pipe_fd, shell);
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
} */

int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	static bool	heredoc_handled;

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	heredoc_handled = false;
	// Handle all heredocs in the pipeline first
    if (!heredoc_handled)
    {
        if (!handle_pipeline_heredocs(shell->cmnd_lst))
            return (0);
        heredoc_handled = true;
    }
	if (!handle_invalid_command(current))
		return (0);
	signal(SIGINT, SIG_IGN);
	if (!init_pipeline(current, pipe_fd, shell))
		return (0);
	if (!create_process(&pid, shell))
	{
		cleanup_pipeline_resources(prev_pipe, pipe_fd);
		return (0);
	}
	if (pid == 0)
		handle_child_process(current, prev_pipe, pipe_fd, shell);
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
}

/* int	masha_setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	(void) prev_pipe;
	(void) last_pid;
	if (pipe(pipe_fd) == -1)
		;//TO DO CLEAN EXIT
	pid = fork();
	if (pid == -1)
		;//TO DO CLEAN EXIT
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execute_single_command(current, shell);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (1);
} */

void	execute_pipeline_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
	{
		perror("redirection failed");
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free(cmd_path);
		exit(1);
	}
	if (!execve(cmd_path, cmd->args, shell->envp))
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free(cmd_path);
		handle_command_errors(cmd_path, cmd->args[0]);
		exit(EXIT_FAILURE);
	}
}
