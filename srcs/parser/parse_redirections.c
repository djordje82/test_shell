/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:05 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/19 17:03:13 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redirection	*create_redirec(t_token_type type, const char *filename)
{
	t_redirection	*new_redir;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
		return (NULL);
	new_redir->filename = ft_strdup(filename);
	new_redir->type = type;
	new_redir->heredoc_processed = false;
	new_redir->heredoc_fd = -1;
	new_redir->next = NULL;
	return (new_redir);
}

int	parse_redirections(t_token **token, t_command *cmd)
{
	t_redirection	*new_redir;
	t_redirection	*last_redir;
	t_token_type	type;

	if (!*token || !cmd)
		return (0);
	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
		return (print_syntx_err("syntax error near unexpected token `newline'",
				NULL));
	*token = (*token)->next;
	new_redir = create_redirec(type, (*token)->value);
	if (!new_redir)
		return (0);
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else
	{
		last_redir = cmd->redirections;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = new_redir;
	}
	*token = (*token)->next;
	return (1);
}
