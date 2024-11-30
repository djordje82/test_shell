/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:05 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/29 12:53:48 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_redirection(t_command *cmd, char *filename, t_token_type type)
{
	if (type == TOKEN_RDRCT_IN || type == TOKEN_HEREDOC)
	{
		free(cmd->infile);
		cmd->infile = filename;
		if (type == TOKEN_RDRCT_IN)
			cmd->in_type = REDIR_INPUT;
		else
			cmd->in_type = REDIR_HEREDOC;
	}
	else
	{
		free(cmd->outfile);
		cmd->outfile = filename;
		if (type == TOKEN_RDRCT_OUT)
			cmd->out_type = REDIR_TRUNC;
		else
			cmd->out_type = REDIR_APPEND;
	}
}

/*This function is used to parse the redirections of a command.*/
int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		print_syntx_err("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
		//return (cleanup_and_exit(ERR_MEM, NULL, 1, NULL));
		return (0);
	set_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
}
