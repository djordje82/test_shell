/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:00 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/19 21:16:50 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_type(t_redirection *redir, int stdin_backup,
		int stdout_backup)
{
	if (!redir->heredoc_processed)
	{
		if (!setup_heredoc(redir))
		{
			restore_std_fds(stdin_backup, stdout_backup);
			return (0);
		}
	}
	if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
	{
		restore_std_fds(stdin_backup, stdout_backup);
		return (0);
	}
	return (1);
}

static int	handle_redirection_type(t_command *cmd, t_redirection *redir,
		int stdin_backup, int stdout_backup)
{
	if (redir->type == TOKEN_HEREDOC)
	{
		if (handle_heredoc_type(redir, stdin_backup, stdout_backup))
			return (1);
	}
	if (redir->type == TOKEN_REDIR_IN)
	{
		if (!handle_input_redirection(redir))
		{
			restore_std_fds(stdin_backup, stdout_backup);
			g_exit_status = 1;
			return (0);
		}
	}
	else if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_APPEND)
	{
		if (!handle_output_redirection(cmd))
		{
			restore_std_fds(stdin_backup, stdout_backup);
			return (0);
		}
	}
	return (1);
}

static int	start_redirections(t_command *cmd, int *stdin_backup,
		int *stdout_backup, t_redirection **redir)
{
	if (!backup_std_fds(stdin_backup, stdout_backup))
		return (0);
	*redir = cmd->redirections;
	return (1);
}

int	setup_redirections(t_command *cmd)
{
	t_redirection	*redir;
	int				stdin_backup;
	int				stdout_backup;

	if (!start_redirections(cmd, &stdin_backup, &stdout_backup, &redir))
		return (0);
	while (redir)
	{
		if (!handle_redirection_type(cmd, redir, stdin_backup, stdout_backup))
			return (0);
		redir = redir->next;
	}
	return (1);
}
