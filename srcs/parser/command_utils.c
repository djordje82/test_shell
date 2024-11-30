/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:57:58 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/30 17:05:36 by jadyar           ###   ########.fr       */
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
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->in_type = 0;
	cmd->out_type = 0;
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
