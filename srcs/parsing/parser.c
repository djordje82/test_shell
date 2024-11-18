/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:03:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:03:41 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_pipe_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
		return (0);
	while (tokens && tokens->next)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next->type == TOKEN_PIPE)
			return (0);
		tokens = tokens->next;
	}
	if (tokens && tokens->type == TOKEN_PIPE)
		return (0);
	return (1);
}

/*This function is used to parse a command from a list of tokens.*/
t_command	*parse_command(t_token **token)
{
	t_command	*cmd;
	t_token		*temp;
	int			i;

	temp = *token;
	while (temp)
		temp = temp->next;
	cmd = create_cmd_node();
	if (!cmd)
		return (NULL);
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if ((*token)->type == TOKEN_RDRCT_IN
			|| (*token)->type == TOKEN_RDRCT_OUT
			|| (*token)->type == TOKEN_APPEND
			|| (*token)->type == TOKEN_HEREDOC)
		{
			if (!parse_redirections(token, cmd))
				return (cleanup_cmd_list(cmd), NULL);
			continue ;  // Add continue here to skip the extra token advance
		}
		else if ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_QUOTE
			|| (*token)->type == TOKEN_DQUOTE)
		{
			if (!parse_cmd_arguments(token, cmd))
				return (cleanup_cmd_list(cmd), NULL);
			continue ;
		}
		if (*token && (*token)->type != TOKEN_PIPE)
			*token = (*token)->next;
	}
	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	return (cmd);
}

static int	build_command_list(t_token *tokens, t_shell *shell)
{
	t_command	*cmd;
	t_token		*current;

	current = tokens;
	shell->cmnd_lst = NULL;
	while (current)
	{
		cmd = parse_command(&current);
		if (!cmd)
		{
			if (shell->cmnd_lst)
				cleanup_cmd_list(shell->cmnd_lst);
			shell->cmnd_lst = NULL;
			return (0);
		}
		add_cmd_node(&shell->cmnd_lst, cmd);
		if (current && current->type == TOKEN_PIPE)
			current = current->next;
	}
	return (1);
}

int	parse_tokens(t_shell *shell)
{
	if (!shell->tokens)
		return (1);
	if (!validate_pipe_syntax(shell->tokens))
	{
		cleanup_and_exit(ERR_SYNTAX_PIPE, NULL, 2, shell);
		g_exit_status = 2; // Set exit status for syntax error
		return (0);        // Return but don't exit
	}
	if (!build_command_list(shell->tokens, shell))
	{
		g_exit_status = 2; // Set exit status for syntax error
		return (0);        // Return but don't exit
	}
	return (1);
}