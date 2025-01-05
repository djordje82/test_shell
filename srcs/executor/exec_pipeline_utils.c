/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:20:10 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/05 10:42:45 by dodordev         ###   ########.fr       */
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

int	create_process(pid_t *pids, t_shell *shell)
{
	*pids = fork();
	if (*pids == -1)
		return (cleanup_and_exit("fork failed", NULL, 1, shell));
	return (1);
}

int	is_parent_only_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0);
}

void	cleanup_pipeline_resources(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe)
		close_pipe_ends(prev_pipe);
	if (pipe_fd)
		close_pipe_ends(pipe_fd);
}
