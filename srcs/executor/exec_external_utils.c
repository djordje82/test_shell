/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:23 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/15 16:18:28 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command_errors(char *cmd_path, char *cmd_name)
{
	struct stat	path_stat;

	if (access(cmd_path, F_OK) != 0)
	{
		print_command_error(cmd_name, ": No such file or directory");
		exit(127);
	}
	if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_command_error(cmd_name, ": Is a directory");
		exit(126);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		print_command_error(cmd_name, ": Permission denied");
		exit(126);
	}
}

void	execute_external_child(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
	{
		perror("redirection failed");
		exit(1);
	}
	execve(cmd_path, cmd->args, shell->envp);
	handle_command_errors(cmd_path, cmd->args[0]);
	exit(g_exit_status);
}

int	print_command_not_found(t_command *cmd)
{
	struct stat	path_stat;

	if (!cmd || !cmd->args || !cmd->args[0] || !cmd->args[0][0])
	{
		ft_putstr_fd("minishell: Invalid cmd\n", STDERR_FILENO);
		return (127);
	}
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (stat(cmd->args[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			print_command_error(cmd->args[0], ": Is a directory");
			return (126);
		}
		print_command_error(cmd->args[0], ": No such file or directory");
		return (127);
	}
	print_command_error(cmd->args[0], ": command not found");
	return (127);
}
