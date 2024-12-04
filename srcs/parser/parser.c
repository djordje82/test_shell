/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:03:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:41:59 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TO DO: SPLIT
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
		if ((*token)->type == TOKEN_REDIR_IN
			|| (*token)->type == TOKEN_REDIR_OUT
			|| (*token)->type == TOKEN_APPEND
			|| (*token)->type == TOKEN_HEREDOC)
		{
			if (!parse_redirections(token, cmd))
			{
				cmd->is_valid = false;
				cleanup_cmd_list(cmd);
				return (NULL);
			}
			continue ;
		}
		else if ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_SQUOTE
			|| (*token)->type == TOKEN_DQUOTE)
		{
			if (!parse_cmd_arguments(token, cmd))
			{
				cmd->is_valid = false;
				cleanup_cmd_list(cmd);
				return (NULL);
			}
			continue ;
		}
		*token = (*token)->next;
	}
	if (!cmd->args || !cmd->args[0])
		cmd->is_valid = false;
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

static int	validate_pipe_syntax(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (tokens->type == TOKEN_PIPE)
	{
		print_syntx_err("syntax error near unexpected token `|'", NULL);
	}
	while (tokens && tokens->next)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next->type == TOKEN_PIPE)
		{
			print_syntx_err("syntax error near unexpected token `|'", NULL);
			return (0);
		}
		tokens = tokens->next;
	}
	if (tokens && tokens->type == TOKEN_PIPE)
	{
		print_syntx_err("syntax error near unexpected token `|'", NULL);
		return (0);
	}
	return (1);
}

int	parse_tokens(t_shell *shell)
{
	if (!shell->tokens)
		return (1);
	if (!validate_pipe_syntax(shell->tokens))
	{
		g_exit_status = 2;
		return (0);
	}
	if (!build_command_list(shell->tokens, shell))
	{
		g_exit_status = 2;
		return (0);
	}
	return (1);
}
