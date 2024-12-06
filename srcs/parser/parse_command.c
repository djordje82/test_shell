/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:15:54 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/06 13:17:18 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_redirection(t_token **token, t_command *cmd)
{
	if (!parse_redirections(token, cmd))
	{
		cmd->is_valid = false;
		cleanup_cmd_list(cmd);
		return (false);
	}
	return (true);
}

static bool	process_token(t_token **token, t_command *cmd)
{
	if ((*token)->type == TOKEN_REDIR_IN || (*token)->type == TOKEN_REDIR_OUT
		|| (*token)->type == TOKEN_APPEND || (*token)->type == TOKEN_HEREDOC)
		return (handle_redirection(token, cmd));
	else if ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_SQUOTE
		|| (*token)->type == TOKEN_DQUOTE)
	{
		if (!parse_cmd_arguments(token, cmd))
		{
			cmd->is_valid = false;
			cleanup_cmd_list(cmd);
			return (false);
		}
		return (true);
	}
	*token = (*token)->next;
	return (true);
}

t_command	*parse_command(t_token **token)
{
	t_command	*cmd;

	if (!token || !*token)
		return (NULL);
	cmd = create_cmd_node();
	if (!cmd)
		return (NULL);
	cmd->is_valid = true;
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (!process_token(token, cmd))
			return (NULL);
	}
	if (!cmd->args || !cmd->args[0])
		cmd->is_valid = false;
	return (cmd);
}
