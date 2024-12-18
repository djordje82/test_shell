/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:03:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/18 18:13:02 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	build_command_list(t_token *tokens, t_shell *shell)
{
	t_command	*cmd;
	t_token		*current;

	current = tokens;
	shell->cmnd_lst = NULL;
	while (current)
	{
		cmd = parse_command(&current, shell);
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
} */

static int build_command_list(t_token *tokens, t_shell *shell)
{
    t_command *cmd;
    t_token *current;

    current = tokens;
    shell->cmnd_lst = NULL;
    while (current)
    {
        cmd = parse_command(&current, shell);
        if (!cmd)
        {
            if (shell->cmnd_lst)
                cleanup_cmd_list(shell->cmnd_lst);
            shell->cmnd_lst = NULL;
            ft_putendl_fd("minishell: failed to parse cmd", STDERR_FILENO);
            return (0);
        }

        // Check for invalid command in pipeline
        if (!cmd->is_valid && current && current->type == TOKEN_PIPE)
        {
            print_command_error(cmd->args[0], ": command not found");
            cleanup_cmd_list(cmd);
            if (shell->cmnd_lst)
                cleanup_cmd_list(shell->cmnd_lst);
            shell->cmnd_lst = NULL;
            g_exit_status = 127;
            return (0);
        }

        // Check for proper command sequence (no empty commands in pipeline)
        if ((!cmd->args || !cmd->args[0]) && current && current->type == TOKEN_PIPE)
        {
            ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
            cleanup_cmd_list(cmd);
            if (shell->cmnd_lst)
                cleanup_cmd_list(shell->cmnd_lst);
            shell->cmnd_lst = NULL;
            g_exit_status = 2;
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
		return (0);
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
		//g_exit_status = 2;
		shell->exit_status = 2;
		return (0);
	}
	if (!build_command_list(shell->tokens, shell))
	{
		g_exit_status = 2;
		return (0);
	}
	return (1);
}
