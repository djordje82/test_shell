/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:15 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:08:14 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function performs cleanup of the shell data. It frees the command list | token list | and clears the history.*/
void	cleanup_shell_data(t_shell *shell)
{
	if (!shell)
		return ;
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
	cleanup_execution_data(shell);
	cleanup_envp(shell);
	//rl_clear_history();
}

/*This function prints an error message and exits the program. It also cleans up the shell data.*/
/* int	cleanup_and_exit(char *err_msg, char *src, int err_code, t_shell *shell)
{
	print_error(err_msg, src);
	if (shell)
	{
		shell->exit_status = err_code;
		cleanup_shell_data(shell);
	}
	exit(err_code);
} */
int cleanup_and_exit(char *err_msg, char *src, int err_code, t_shell *shell)
{
    print_error(err_msg, src);
    
    if (shell)
    {
        shell->exit_status = err_code;
        cleanup_shell_data(shell);
    }
    
    exit(err_code);
}