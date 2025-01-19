/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:57:58 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/19 18:18:02 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_cmd_node(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->is_valid = false;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

void	add_cmd_node(t_command **cmd_list, t_command *new_cmd)
{
	t_command	*current;

	if (!new_cmd)
	{
		perror("stop trying to add NULL cmd node");
		return ;
	}
	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		new_cmd->prev = NULL;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
	new_cmd->prev = current;
}

int	is_empty_cmd(t_shell *shell)
{
	if (shell->cmd_lst->args && shell->cmd_lst->args[0]
		&& shell->cmd_lst->args[0][0] == 0)
	{
		g_exit_status = 0;
		reset_shell_state(shell);
		return (1);
	}
	return (0);
}
