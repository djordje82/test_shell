/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:39 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:08:12 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function frees process ID's and pipe arrays from the shell struct.*/
void	cleanup_execution_data(t_shell *shell)
{
	if (shell->pid)
	{
		free(shell->pid);
		shell->pid = NULL;
	}
	if (shell->pipe)
	{
		ft_free_array((void **)shell->pipe, shell->n_cmnds + 1);
		shell->pipe = NULL;
	}
}

/*This function frees the environment variables array.*/
void	cleanup_envp(t_shell *shell)
{
	if (shell->envp)
		ft_free_array((void **)shell->envp, -1);
	shell->envp = NULL;
}

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