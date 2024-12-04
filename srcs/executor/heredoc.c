/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:46:40 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:47:36 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_to_heredoc(int fd, char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (write(fd, line, len) == -1)
		return (0);
	if (write(fd, "\n", 1) == -1)
		return (0);
	free(line);
	return (1);
}

int	setup_heredoc(t_command *cmd)
{
	int		heredoc_pipe[2];
	char	*line;
	size_t	len_delimiter;

	if (!create_pipe(heredoc_pipe, NULL))
		return (0);
	len_delimiter = ft_strlen(cmd->infile);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == len_delimiter && ft_strncmp(line, cmd->infile,
				len_delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(heredoc_pipe[1], line);
	}
	close(heredoc_pipe[1]);
	if (!setup_pipe_io(heredoc_pipe[0], -1))
		return (0);
	return (1);
}
