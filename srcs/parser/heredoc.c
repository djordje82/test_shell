/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/08 22:17:08 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_heredoc(t_command *cmd)
{
	t_redirection	*redir;
	int				heredoc_pipe[2];
	bool			success;

	redir = cmd->redirections;
	success = true;
	while (redir && success)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			success = setup_single_heredoc(redir, heredoc_pipe);
			if (!success)
				break ;
		}
		redir = redir->next;
	}
	return (success);
}
