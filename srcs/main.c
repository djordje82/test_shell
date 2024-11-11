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

void cleanup_command_data(t_shell *shell)
{
    if (shell->cmnd_lst)
    {
        free_cmd_list(shell->cmnd_lst);
        shell->cmnd_lst = NULL;
    }
    if (shell->tokens)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
    }
}

void handle_input_line(char *input, t_shell *shell)
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
        cleanup_command_data(shell);
    }
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

int main(int argc, char **argv, char **env)
{
    t_shell shell;
    int exit_status;

    if (init_minishell(&shell, env, argc, argv) != 0)
        return (1);

    shell_loop(&shell);

    exit_status = shell.exit_status;
    cleanup_shell(&shell);
    return (exit_status);
}

/*int main(int argc, char **argv, char **env)
{
    t_shell shell;
    //char *input;
    int exit_status;

    if (init_minishell(&shell, env, argc, argv) != 0)
        return (1);

    while (shell.running)
    {
        input = readline(PROMPT);
        if (!input)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }

        if (*input)
        {
            add_history(input);
            shell.tokens = tokenize_input(input, &shell);
            if (shell.tokens && parse_tokens(&shell))
            {
                shell.exit_status = execute_commands(&shell);
                g_exit_status = shell.exit_status;
            }
            if (shell.cmnd_lst)
            {
                free_cmd_list(shell.cmnd_lst);
                shell.cmnd_lst = NULL;
            }
            free_tokens(shell.tokens);
            shell.tokens = NULL;
        }
        free(input);
    }

    exit_status = shell.exit_status;
    cleanup_shell(&shell);
    return (exit_status);
}*/