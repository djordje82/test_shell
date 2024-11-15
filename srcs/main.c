/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:11 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:01:16 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
