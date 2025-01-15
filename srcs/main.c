/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:11 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 14:19:15 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void verify_stdin_state(void)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fprintf(stderr, "DEBUG: Verifying STDIN state:\n");
    fprintf(stderr, "DEBUG: - STDIN flags: %d\n", flags);
    fprintf(stderr, "DEBUG: - STDIN fileno: %d\n", STDIN_FILENO);
    
    // Test if STDIN is valid
    if (flags == -1) {
        fprintf(stderr, "DEBUG: STDIN flags check failed, errno: %d (%s)\n", 
                errno, strerror(errno));
    }

    // Try to duplicate STDIN to verify its validity
    int dup_fd = dup(STDIN_FILENO);
    fprintf(stderr, "DEBUG: - STDIN dup test returned: %d\n", dup_fd);
    if (dup_fd != -1) {
        close(dup_fd);
    }

    // Test if we can read from STDIN
    char test_buf[1];
    int ret = read(STDIN_FILENO, test_buf, 0);
    fprintf(stderr, "DEBUG: - STDIN read test returned: %d\n", ret);
    if (ret == -1) {
        fprintf(stderr, "DEBUG: - Read test failed, errno: %d (%s)\n", 
                errno, strerror(errno));
    }
}

void process_shell_input(char *input, t_shell *shell)
{
    fprintf(stderr, "DEBUG: Starting to process input: '%s'\n", input);
    fprintf(stderr, "DEBUG: Initial STDIN state check:\n");
    verify_stdin_state();

    if (*input)
    {
        add_history(input);
        shell->tokens = tokenize_input(input, shell);
        if (!shell->tokens)
        {
            fprintf(stderr, "DEBUG: Tokenization failed, STDIN state:\n");
            verify_stdin_state();
            if (g_exit_status == 0)
                g_exit_status = 2;
            reset_shell_state(shell);
            return;
        }

        if (!parse_tokens(shell))
        {
            fprintf(stderr, "DEBUG: Parsing failed, STDIN state:\n");
            verify_stdin_state();
            if (g_exit_status == 0)
                g_exit_status = 2;
            reset_shell_state(shell);
            shell->exit_status = 1;
            return;
        }

        fprintf(stderr, "DEBUG: Before execute_commands, STDIN state:\n");
        verify_stdin_state();

        shell->exit_status = execute_commands(shell);
        
        fprintf(stderr, "DEBUG: After execute_commands, exit_status: %d\n", 
                shell->exit_status);
        fprintf(stderr, "DEBUG: Post-execution STDIN state:\n");
        verify_stdin_state();

        g_exit_status = shell->exit_status;
        reset_shell_state(shell);
    }

    fprintf(stderr, "DEBUG: Final STDIN state check:\n");
    verify_stdin_state();
    fprintf(stderr, "DEBUG: Finished processing input\n");
}

/* void	process_shell_input(char *input, t_shell *shell)
{
	fprintf(stderr, "DEBUG: Starting to process input: '%s'\n", input);
	if (*input)
	{
		add_history(input);
		shell->tokens = tokenize_input(input, shell);
		if (!shell->tokens)
		{
			fprintf(stderr, "DEBUG: Tokenization failed\n");
			if (g_exit_status == 0)
				g_exit_status = 2;
			reset_shell_state(shell);
			return ;
		}
		if (!parse_tokens(shell))
		{
			fprintf(stderr, "DEBUG: Parsing failed\n");
			if (g_exit_status == 0)
				g_exit_status = 2;
			reset_shell_state(shell);
			shell->exit_status = 1;
			return ;
		}
		fprintf(stderr, "DEBUG: Before execute_commands\n");
		shell->exit_status = execute_commands(shell);
		fprintf(stderr, "DEBUG: After execute_commands, exit_status: %d\n", shell->exit_status);
		g_exit_status = shell->exit_status;
		reset_shell_state(shell);
		fprintf(stderr, "DEBUG: Shell state reset complete\n");
	}
	fprintf(stderr, "DEBUG: Finished processing input\n");
} */

void	run_shell_loop(t_shell *shell)
{
	char	*input;
	int		stdin_status;

	while (shell->running)
	{
		fprintf(stderr, "DEBUG: Shell loop - checking STDIN status\n");
        stdin_status = fcntl(STDIN_FILENO, F_GETFL);
        fprintf(stderr, "DEBUG: STDIN flags: %d\n", stdin_status);
		input = readline("minishell$ ");
		if (!input)
		{
			fprintf(stderr, "DEBUG: NULL input received, STDIN status: %d\n", 
                    fcntl(STDIN_FILENO, F_GETFL));
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 5);
			cleanup_shell_data(shell);
			rl_clear_history();
			exit(shell->exit_status);
		}
		process_shell_input(input, shell);
		free(input);
		fprintf(stderr, "DEBUG: Shell loop iteration end, running: %d\n", shell->running);
	}
}

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
