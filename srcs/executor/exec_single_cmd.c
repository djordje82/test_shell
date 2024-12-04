/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:09:35 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:49:56 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_empty_command(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i + 1])
	{
		args[i] = args[i + 1];
		i++;
	}
	args[i] = NULL;
}

static int	execute_command_type(t_command *cmd, t_shell *shell, int stdin_fd,
		int stdout_fd)
{
	int	status;

	if (is_builtin(cmd->args[0]))
	{
		status = handle_builtin_cmd(cmd, shell);
		if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
		{
			restore_std_fds(stdin_fd, stdout_fd);
			exit(status);
		}
	}
	else
		status = handle_external_cmd(cmd, shell);
	return (status);
}

int	execute_single_command(t_command *cmd, t_shell *shell)
{
	int	stdin_fd;
	int	stdout_fd;
	int	status;

	if (!cmd || !cmd->args)
		return (0);
	if (cmd->args[0] && !cmd->args[0][0])
		cleanup_empty_command(cmd->args);
	if (!cmd->args[0])
		return (0);
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (stdin_fd == -1 || stdout_fd == -1)
	{
		perror("dup fail");
		return (1);
	}
	if (!setup_redirections(cmd))
	{
		restore_std_fds(stdin_fd, stdout_fd);
		return (g_exit_status);
	}
	status = execute_command_type(cmd, shell, stdin_fd, stdout_fd);
	restore_std_fds(stdin_fd, stdout_fd);
	return (status);
}
