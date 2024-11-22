/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:23 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:25 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function handles wildcard expansion. It expands wildcard characters in command arguments and updates the command structure.*/
/* int	handle_wildcard_expansion(t_command *cmd, t_shell *shell)
{
	char	**expanded_args;
	int		i;
	int		expanded_count;

	if (!cmd || !cmd->args || !shell)
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '*'))
		{
			expanded_args = expand_wildcards(cmd->args[i]);
			if (!expanded_args)
				continue ;
			expanded_count = update_command_args(cmd, expanded_args, i, shell);
			if (expanded_count < 0)
			{
				ft_free_array((void **)expanded_args, -1);
				return (0);
			}
			i += expanded_count - 1;
			ft_free_array((void **)expanded_args, -1);
		}
		i++;
	}
	return (1);
} */

void	handle_command_errors(char *cmd_path, char *cmd_name) // TODO: Remove use of stat() function
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			print_command_error(cmd_name, ": Is a directory");
			exit(126);
		}
		if (access(cmd_path, X_OK | F_OK) != 0)
		{
			print_command_error(cmd_name, ": Permission denied");
			exit(126);
		}
	}
	print_command_error(cmd_name, ": No such file or directory");
	exit(127);
}

/*This function executes an external command in a child process. It sets up child signals, handles redirections, and executes the command.*/
void	execute_external_child(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
		exit(1);
	execve(cmd_path, cmd->args, shell->envp);
	handle_command_errors(cmd_path, cmd->args[0]);
}

/*This function prints a command not found error. It checks if the command is the first in a pipeline and prints an error message accordingly.*/
int	print_command_not_found(t_command *cmd)
{
	if (!cmd->prev && cmd->args[0][0])
	{
		if (ft_strchr(cmd->args[0], '/'))
			print_command_error(cmd->args[0], ": No such file or directory");
		else
			print_command_error(cmd->args[0], ": command not found");
	}
	return (127);
}
