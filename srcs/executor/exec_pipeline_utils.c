/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:20:10 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 17:26:43 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	restore_std_fds(int stdin_fd, int stdout_fd)
{
	int	result;

	result = 1;
	if (dup2(stdin_fd, STDIN_FILENO) == -1)
	{
		result = 0;
	}
	if (dup2(stdout_fd, STDOUT_FILENO) == -1)
	{
		result = 0;
	}
	close(stdin_fd);
	close(stdout_fd);
	return (result);
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

void cleanup_pipeline_resources(int *prev_pipe, int *pipe_fd)
{
    if (prev_pipe)
    {
        if (prev_pipe[0] != -1)
        {
            close(prev_pipe[0]);
            prev_pipe[0] = -1;
        }
        if (prev_pipe[1] != -1)
        {
            close(prev_pipe[1]);
            prev_pipe[1] = -1;
        }
    }
    
    if (pipe_fd)
    {
        if (pipe_fd[0] != -1)
        {
            close(pipe_fd[0]);
            pipe_fd[0] = -1;
        }
        if (pipe_fd[1] != -1)
        {
            close(pipe_fd[1]);
            pipe_fd[1] = -1;
        }
    }
}
