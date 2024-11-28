/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:11 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 16:11:59 by dodordev         ###   ########.fr       */
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

/*This function processes a single line of input from the user. \ 
It checks if the input is not empty | Adds it to the history \ 
Tokenizes the input | Parses the tokens | Executes the commands \ 
Cleans up the command data.*/
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

/*This function starts the shell loop. \ 
It reads input from the user | processes the input | and frees the input.*/
void	run_shell_loop(t_shell *shell)
{
	char	*input;

	while (shell->running)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 6);
			exit(0);
		}
		process_shell_input(input, shell);
		free(input);
	}
}

/*This function initializes the shell struct.*/
void	initialize_shell(t_shell *shell, char **envp)
{
	shell->cmnd_lst = NULL;
	shell->tokens = NULL;
	shell->pipe = NULL;
	shell->pid = NULL;
	shell->envp = NULL;
	shell->n_cmnds = 0;
	shell->exit_status = 0;
	shell->running = true;
	shell->envp = copy_env(envp);
	if (!shell->envp)
		exit(EXIT_FAILURE);
	update_shell_level(shell);
}

/*This function initializes entire minishell program. \ 
It checks if the number of arguments is not 1 \ 
Initializes the shell struct \ Sets up the signals \ 
Sets the running flag to true.*/

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		exit_status;

	if (argc != 1)
		return (cleanup_and_exit(ERR_BATCH, NULL, 1, NULL));
	(void)argv;
	initialize_shell(&shell, env);
	setup_signals();
	run_shell_loop(&shell);
	exit_status = shell.exit_status;
	cleanup_shell_data(&shell);
	return (exit_status);
}
