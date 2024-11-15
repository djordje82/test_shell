#include "minishell.h"

void	init_shell_struct(t_shell *shell, char **envp)
{
	//printf("Debug: Initializing shell struct\n");
	shell->cmnd_lst = NULL;
	shell->tokens = NULL;
	shell->pipe = NULL;
	shell->pid = NULL;
	shell->n_cmnds = 0;
	shell->exit_status = 0;
	shell->running = true;
	
	// Create a copy of the environment
	shell->envp = copy_env(envp);
	if (!shell->envp)
	{
		//printf("Debug: Failed to copy environment\n");
		exit(EXIT_FAILURE);
	}
	
	//printf("Debug: Shell struct initialization complete\n");
}

int	is_valid_shell_var(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(name[0]))
		return (0);

	i = 0;
	while (name[i])
	{
		if (name[i] == '=')
		{
			break;
		}
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}

	return (1);
}

void shell_loop(t_shell *shell)
{
    char *input;

    while (shell->running)
    {
        input = readline(PROMPT);
        if (!input)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }
        handle_input_line(input, shell);
        free(input);
    }
}

int  init_minishell(t_shell *shell, char **env, int argc, char **argv)
{
    if (argc != 1)
        return (exit_error("minishell: too many arguments\n", NULL, 1, NULL));
    (void)argv;

    ft_memset(shell, 0, sizeof(t_shell));  // Initialize all fields to NULL/0
    init_shell_struct(shell, env);

    setup_signals();
    shell->running = true;
    return (0);
}