/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:15 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 12:59:05 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function frees a linked list of tokens. It iterates through the list,
	freeing each token's value and then the token itself.*/
void	cleanup_token_list(t_token *tokens)
{
	t_token *current;
	t_token *next;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

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
	// rl_clear_history();
}

void	print_error_msg(char *err_msg, char *src)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (src)
	{
		ft_putstr_fd(src, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
}

int	cleanup_and_exit(char *err_msg, char *src, int err_code, t_shell *shell)
{
	print_error_msg(err_msg, src);

	if (shell)
	{
		shell->exit_status = err_code;
		cleanup_shell_data(shell);
	}

	exit(err_code);
}