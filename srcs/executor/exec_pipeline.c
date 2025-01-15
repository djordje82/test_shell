/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 18:05:02 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_redirection *redir;

	redir = current->redirections;
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC && !redir->heredoc_processed)
        {
            if (!setup_heredoc(redir))
            {
                return 0;
            }
        }
        redir = redir->next;
    }

	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && !create_pipe(pipe_fd, shell))
	{
		return (0);
	}
	if (!create_process(&pid, shell))
	{
		cleanup_pipeline_resources(prev_pipe, pipe_fd);
		return (0);
	}
	if (pid == 0)
	{
		handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
	}

	if (!current->next)
	{
		*last_pid = pid;
	}
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
}

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
		handle_command_errors(cmd_path, cmd->args[0]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}
