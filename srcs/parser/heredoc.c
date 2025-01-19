/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/19 16:30:43 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	cleanup_heredoc(int *heredoc_pipe, bool is_last)
{
	if (heredoc_pipe[1] != -1)
	{
		close(heredoc_pipe[1]);
		heredoc_pipe[1] = -1;
	}
	if (is_last)
	{
		if (heredoc_pipe[0] != -1)
		{
			if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
			{
				close(heredoc_pipe[0]);
				heredoc_pipe[0] = -1;
				return (0);
			}
		}
	}
	if (heredoc_pipe[0] != -1)
	{
		close(heredoc_pipe[0]);
		heredoc_pipe[0] = -1;
	}
	return (1);
} */

/* int	setup_heredoc(t_redirection *redir)
{
	int		heredoc_pipe[2];
	size_t	len_delimiter;

	if (redir->heredoc_processed)
		return (1);
	if (!create_pipe(heredoc_pipe, NULL))
		return (0);
	len_delimiter = ft_strlen(redir->filename);
	setup_heredoc_signals();
	if (!process_heredoc_lines(heredoc_pipe, redir, len_delimiter))
		return (0);
	redir->heredoc_processed = true;
	return (cleanup_heredoc(heredoc_pipe, true));
} */

int setup_heredoc(t_redirection *redir)
{
    int heredoc_pipe[2];
    size_t len_delimiter;

    // printf("DEBUG: Setting up heredoc for %s\n", redir->filename);
    
    if (redir->heredoc_processed)
        return (1);
        
    if (!create_pipe(heredoc_pipe, NULL)) {
        // printf("DEBUG: Pipe creation failed\n");
        return (0);
    }
    
    // printf("DEBUG: Created pipe: read=%d, write=%d\n", 
    //        heredoc_pipe[0], heredoc_pipe[1]);
           
    len_delimiter = ft_strlen(redir->filename);
    setup_heredoc_signals();
    
    if (!process_heredoc_lines(heredoc_pipe, redir, len_delimiter)) {
        // printf("DEBUG: Processing heredoc lines failed\n");
		close_pipe_ends(heredoc_pipe);
        return (0);
    }
    
    redir->heredoc_fd = heredoc_pipe[0];  // Store the read end
    close(heredoc_pipe[1]);  // Close write end
    redir->heredoc_processed = true;
    return (1);
}