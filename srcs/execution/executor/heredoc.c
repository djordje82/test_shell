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

/*This function sets up a heredoc for a command. It creates a pipe, reads lines from the user, and writes them to the pipe until the delimiter is matched.*/
int	setup_heredoc(t_command *cmd)
{
	int		heredoc_pipe[2];
	char	*line;
	size_t	len_delimiter;

	if (pipe(heredoc_pipe) == -1)
		return (cleanup_and_exit(ERR_PIPE, NULL, 1, NULL));
	len_delimiter = ft_strlen(cmd->infile);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == len_delimiter
			&& ft_strncmp(line, cmd->infile, len_delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_to_heredoc(heredoc_pipe[1], line);
	}
	close(heredoc_pipe[1]);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	close(heredoc_pipe[0]);
	return (1);
}