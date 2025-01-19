/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/19 17:17:08 by dodordev         ###   ########.fr       */
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
    int heredoc_pipe[2] = {-1, -1};  // Initialize to invalid
    size_t len_delimiter;
    
    // printf("DEBUG: Setup heredoc starting for %s (current fd: %d)\n", 
    //        redir->filename, redir->heredoc_fd);
    
    if (redir->heredoc_processed)
        return (1);
        
    if (!create_pipe(heredoc_pipe, NULL)) {
        return (0);
    }
    
    // printf("DEBUG: Created new pipe: [%d, %d]\n", heredoc_pipe[0], heredoc_pipe[1]);
           
    len_delimiter = ft_strlen(redir->filename);
    setup_heredoc_signals();
    
    if (!process_heredoc_lines(heredoc_pipe, redir, len_delimiter)) {
        // printf("DEBUG: process_heredoc_lines failed, closing pipe: [%d, %d]\n", 
        //        heredoc_pipe[0], heredoc_pipe[1]);
        if (heredoc_pipe[0] >= 0)
            close(heredoc_pipe[0]);
        if (heredoc_pipe[1] >= 0)
            close(heredoc_pipe[1]);
        return (0);
    }
    
    // printf("DEBUG: Storing read end %d in redirection\n", heredoc_pipe[0]);
    redir->heredoc_fd = heredoc_pipe[0];  // Store the read end
    
    if (heredoc_pipe[1] >= 0) {
        // printf("DEBUG: Closing write end %d\n", heredoc_pipe[1]);
        close(heredoc_pipe[1]);  // Close write end
    }
    redir->heredoc_processed = true;
    return (1);
}