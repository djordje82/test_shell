/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:05 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:43:30 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_redirection(t_command *cmd, char *filename, t_token_type type)
{
	if (!filename)
		return ;
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
	{
		free(cmd->infile);
		cmd->infile = filename;
		if (type == TOKEN_REDIR_IN)
			cmd->in_type = REDIR_INPUT;
		else
			cmd->in_type = REDIR_HEREDOC;
	}
	else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
	{
		free(cmd->outfile);
		cmd->outfile = filename;
		if (type == TOKEN_REDIR_OUT)
			cmd->out_type = REDIR_TRUNC;
		else
			cmd->out_type = REDIR_APPEND;
	}
	else 
		free(filename);
}

int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	if (!*token || !(*token) || !cmd)
		return (0);
	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		print_syntx_err("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
	{
		print_syntx_err("malloc failed", NULL);
		return (0);
	}
	set_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
}
