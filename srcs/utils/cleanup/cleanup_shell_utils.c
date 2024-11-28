/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:39 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 13:54:17 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function frees a linked list of commands.*/
void	cleanup_cmd_list(t_command *cmd)
{
	t_command	*temp;

	if (!cmd)
		return ;
	while (cmd)
	{
		temp = cmd->next;
		if (cmd->args)
			ft_free_array((void **)cmd->args, -1);
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		free(cmd);
		cmd = temp;
	}
}

/*This function frees a linked list of tokens.*/
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

/*This function performs cleanup of the shell data. \ 
It frees the command list | token list | and clears the history.*/
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
	rl_clear_history();
}