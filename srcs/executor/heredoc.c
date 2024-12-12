/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:31:34 by jadyar            #+#    #+#             */
/*   Updated: 2024/12/13 11:31:49 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_line(char *line, const char *delimiter,
		size_t len_delimiter)
{
	size_t	line_len;

	if (!line)
		return (-1);
	line_len = ft_strlen(line);
	if (line_len > 0 && line[line_len - 1] == '\n')
		line[line_len - 1] = '\0';
	if (ft_strlen(line) == len_delimiter && ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}

static int	write_to_heredoc(int fd, char *line)
{
	size_t	len;

	if (!line)
		return (0);
	len = ft_strlen(line);
	if (write(fd, line, len) == -1 || write(fd, "\n", 1) == -1)
	{
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

static int	cleanup_heredoc(int *heredoc_pipe, bool is_last)
{
	close(heredoc_pipe[1]);
	if (is_last)
	{
		if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
		{
			close(heredoc_pipe[0]);
			return (0);
		}
	}
	close(heredoc_pipe[0]);
	return (1);
}

int	setup_heredoc(t_command *cmd) fd779f71879c924d2595fbbfced2ad9bb6da83cf
{
	int heredoc_pipe[2];
	char *line;
	size_t len_delimiter;
	int heredoc_status;

	if (!create_pipe(heredoc_pipe, NULL))
		return (0);
	len_delimiter = ft_strlen(cmd->infile);
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		heredoc_status = handle_heredoc_line(line, cmd->infile, len_delimiter);
		if (heredoc_status == -1)
		{
			close_pipe_ends(heredoc_pipe);
			return (0);
		}
		else if (heredoc_status == 0)
		{
			return (cleanup_heredoc(heredoc_pipe, true));
		}
		if (!write_to_heredoc(heredoc_pipe[1], line))
		{
			close_pipe_ends(heredoc_pipe);
			return (0);
		}
	}
	return (cleanup_heredoc(heredoc_pipe, true));
}
