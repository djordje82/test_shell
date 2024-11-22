/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:57:58 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:58:00 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_cmd_node(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->in_type = 0;
	cmd->out_type = 0;
	cmd->is_valid = false;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd_node(t_command **cmd_list, t_command *new_cmd)
{
	t_command	*current;

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

/* bool validate_command(t_command *cmd, t_shell *shell)
{
    char *cmd_path;

    // No command or empty command
    if (!cmd || !cmd->args || !cmd->args[0])
    {
        cmd->is_valid = false;
        return (false);
    }

    // Check if it's a builtin
    if (is_builtin(cmd->args[0]))
    {
        cmd->is_valid = true;
        return (true);
    }

    // Check if external command exists
    cmd_path = find_command(cmd->args[0], shell->envp);
    if (!cmd_path)
    {
        cmd->is_valid = false;
        return (false);
    }

    free(cmd_path);
    cmd->is_valid = true;
    return (true);
} */