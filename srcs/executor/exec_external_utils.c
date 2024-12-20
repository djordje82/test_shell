/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:23 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/19 14:49:47 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_command_errors(char *cmd_path)
{
	struct stat	path_stat;

	if (access(cmd_path, F_OK) != 0)
		return (127);
	if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		return (126);
	if (access(cmd_path, X_OK) != 0)
		return (126);
	return (0);
}

void	handle_command_errors(char *cmd_path, char *cmd_name)
{
	int			error_code;
	struct stat	path_stat;

	error_code = check_command_errors(cmd_path);
	if (error_code == 127)
		print_command_error(cmd_name, ": No such file or directory");
	else if (error_code == 126)
	{
		stat(cmd_path, &path_stat);
		if (S_ISDIR(path_stat.st_mode))
			print_command_error(cmd_name, ": Is a directory");
		else
			print_command_error(cmd_name, ": Permission denied");
	}
	exit(error_code);
}

int	print_command_not_found(t_command *cmd)
{
	int			error_code;

	if (!cmd || !cmd->args || !cmd->args[0] || !cmd->args[0][0])
	{
		ft_putstr_fd("minishell: Invalid cmd\n", STDERR_FILENO);
		return (127);
	}
	if (ft_strchr(cmd->args[0], '/'))
	{
		error_code = check_command_errors(cmd->args[0]);
		if (error_code == 126)
			print_command_error(cmd->args[0], ": Permission denied");
		else
			print_command_error(cmd->args[0], ": No such file or directory");
		return (error_code);
	}
	print_command_error(cmd->args[0], ": command not found");
	return (127);
}
