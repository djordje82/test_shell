/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:39 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:42 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external(t_command *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!handle_wildcard_expansion(cmd, shell))
		return (1);
	if (!cmd->args || !cmd->args[0] || !cmd->args[0][0])
		return (0);
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
		return (handle_command_not_found(cmd));
	if (cmd->prev || cmd->next)
	{
		setup_pipeline_execution(cmd, cmd_path, shell);
		free(cmd_path);
		return (0);
	}
	return (execute_external_command(cmd, cmd_path, shell));
}
