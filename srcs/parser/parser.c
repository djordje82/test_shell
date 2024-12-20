/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:03:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/11 19:40:17 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			ft_putendl_fd("minishell: failed to parse cmd", STDERR_FILENO);
			return (0);
		}
		add_cmd_node(&shell->cmnd_lst, cmd);
		if (current && current->type == TOKEN_PIPE)
			current = current->next;
	}
	return (1);
}

static int	is_pipe_token(t_token *token)
{
	return (token && token->type == TOKEN_PIPE);
}

static int	validate_pipe_syntax(t_token *tokens)
{
	if (!tokens)
	{
		ft_putendl_fd("minishell: syntax error: empty", STDERR_FILENO);
		return (0);
	}
	if (is_pipe_token(tokens))
		return (print_syntx_err("syntax error near unexpected \
			token `|'", NULL));
	while (tokens && tokens->next)
	{
		if (is_pipe_token(tokens) && is_pipe_token(tokens->next))
			return (print_syntx_err("syntax error near \
				unexpected token `|'", NULL));
		tokens = tokens->next;
	}
	if (is_pipe_token(tokens))
		return (print_syntx_err("syntax error near \
			unexpected token `|'", NULL));
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
