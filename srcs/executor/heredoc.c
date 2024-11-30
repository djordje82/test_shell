#include "minishell.h"

/*This function writes a line to a heredoc pipe.*/
static int	write_to_heredoc(int fd, char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (write(fd, line, len) == -1)
	{
		free(line);
		return (0);
	}
	if (write(fd, "\n", 1) == -1)
	{
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

/*This function sets up a heredoc for a command. It creates a pipe,
	reads lines from the user,
	and writes them to the pipe until the delimiter is matched.*/

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
		{
			perror("readline failed");
			break ;
		}
		if (ft_strlen(line) == len_delimiter && ft_strncmp(line, cmd->infile,
				len_delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!write_to_heredoc(heredoc_pipe[1], line))
		{
			perror("write to Heredoc failed");
			close_pipe_ends(heredoc_pipe);
			return (0);
		}
	}
	close(heredoc_pipe[1]);
	if (!setup_pipe_io(heredoc_pipe[0], -1))
	{
		close(heredoc_pipe[0]);
		return (0);
	}
	return (1);
}
