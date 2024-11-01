static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = env;
	shell->commands = NULL;
	shell->tokens = NULL;
	shell->envp = envp;
	shell->exit_status = 0;
	shell->running = true;
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*input;

	if (argc != 1)
		return (exit_error("minishell: too many arguments\n", NULL, 1, NULL));
	(void)argv;
	init_shell(&shell, env);
	setup_signals();
	while (shell.running)
	{
		input = readline(PROMPT);
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*input)
		{
			add_history(input);
			shell.tokens = tokenize_input(input, &shell);
			if (shell.tokens && parse_tokens(&shell))
			{
				execute_commans(&shell);
				free_cmd_list(shell.cmnd_list);
				shell.cmnd_list = NULL;
			}
			free_tokens(shell.tokens);
			shell.tokens = NULL;
		}
		free(input);
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}
