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

/*This function frees a linked list of commands. It iterates through the list,
	freeing each command's arguments | infiles | outfiles | and then the command itself.*/
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
	rl_clear_history();
}
