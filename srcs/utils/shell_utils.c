/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:48:09 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:07:36 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function initializes the shell struct. It sets the command list and tokens to NULL. It also copies the environment variables to the shell struct.*/
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
}

/*This function checks if a variable name is valid. It checks if the name is not empty | if the first character is a digit | if the name contains an equal sign.*/
int	validate_shell_var(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(name[0]))
		return (0);
	i = 0;
	while (name[i])
	{
		if (name[i] == '=')
		{
			break ;
		}
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*This function starts the shell loop. It reads input from the user | processes the input | and frees the input.*/
void	run_shell_loop(t_shell *shell)
{
	char	*input;

	while (shell->running)
	{
		input = readline(PROMPT);
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		process_shell_input(input, shell);
		free(input);
	}
}

/*This function initializes entire minishell program. It checks if the number of arguments is not 1 | Sets all fields to NULL/0 | Initializes the shell struct | Sets up the signals | and sets the running flag to true.*/
int	setup_minishell(t_shell *shell, char **env, int argc, char **argv)
{
	if (argc != 1)
		return (cleanup_and_exit("minishell: too many arguments\n", NULL, 1, NULL));
	(void)argv;

	ft_memset(shell, 0, sizeof(t_shell));
	initialize_shell(shell, env);

	setup_signals();
	shell->running = true;
	return (0);
}