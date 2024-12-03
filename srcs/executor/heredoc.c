#include "minishell.h"

/*This function writes a line to a heredoc pipe.*/
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

/*This function sets up a heredoc for a command. It creates a pipe,
	reads lines from the user,
	and writes them to the pipe until the delimiter is matched.*/

int	setup_heredoc(t_command *cmd)
{
	int		heredoc_pipe[2];
	char	*line;
	size_t	len_delimiter;

	// Use our unified pipe creation function
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
	// Close write end first
	close(heredoc_pipe[1]);
	// Setup the pipe for input and close remaining fds
	if (!setup_pipe_io(heredoc_pipe[0], -1))
		return (0);
	return (1);
}
