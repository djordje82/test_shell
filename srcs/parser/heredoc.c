/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/19 18:22:49 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_heredoc(t_redirection *redir)
{
	int		heredoc_pipe[2];
	size_t	len_delimiter;

	heredoc_pipe[0] = -1;
	heredoc_pipe[1] = -1;
	if (redir->heredoc_processed)
		return (1);
	if (!create_pipe(heredoc_pipe, NULL))
		return (0);
	len_delimiter = ft_strlen(redir->filename);
	setup_heredoc_signals();
	if (!process_heredoc_lines(heredoc_pipe, redir, len_delimiter))
	{
		if (heredoc_pipe[0] >= 0)
			close(heredoc_pipe[0]);
		if (heredoc_pipe[1] >= 0)
			close(heredoc_pipe[1]);
		return (0);
	}
	redir->heredoc_fd = heredoc_pipe[0];
	if (heredoc_pipe[1] >= 0)
		close(heredoc_pipe[1]);
	redir->heredoc_processed = true;
	return (1);
}
