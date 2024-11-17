/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:11 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:07:49 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function resets the shell state by freeing the command list and the tokens. Frees command list and tokens | Resets command list and tokens to NULL.*/
void	reset_shell_state(t_shell *shell)
{
	if (shell->cmnd_lst)
	{
		cleanup_cmd_list(shell->cmnd_lst);
		shell->cmnd_lst = NULL;
	}
	if (shell->tokens)
	{
		cleanup_token_list(shell->tokens);
		shell->tokens = NULL;
	}
}

/*This function processes a single line of input from the user. It checks if the input is not empty | Adds it to the history | Tokenizes the input | Parses the tokens | Executes the commands | Cleans up the command data.*/
void	process_shell_input(char *input, t_shell *shell)
{
	if (*input)
	{
		add_history(input);
		shell->tokens = tokenize_input(input, shell);
		if (shell->tokens && parse_tokens(shell))
		{
			shell->exit_status = execute_commands(shell);
			g_exit_status = shell->exit_status;
		}
		reset_shell_state(shell);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		exit_status;

	if (setup_minishell(&shell, env, argc, argv) != 0)
		return (1);
	run_shell_loop(&shell);
	exit_status = shell.exit_status;
	cleanup_shell_data(&shell);
	return (exit_status);
}
