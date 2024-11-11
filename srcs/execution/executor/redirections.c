#include "minishell.h"

static int handle_heredoc(t_command *cmd)
{
    int     heredoc_pipe[2];
    char    *line;
    size_t  len_delimiter;

    if (pipe(heredoc_pipe) == -1)
        return (exit_error(ERR_PIPE, NULL, 1, NULL));

    len_delimiter = ft_strlen(cmd->infile);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
            
        // Check if line matches delimiter exactly
        if (ft_strlen(line) == len_delimiter && 
            ft_strncmp(line, cmd->infile, len_delimiter) == 0)
        {
            free(line);
            break;
        }
        
        // Write line to pipe
        write(heredoc_pipe[1], line, ft_strlen(line));
        write(heredoc_pipe[1], "\n", 1);
        free(line);
    }
    
    close(heredoc_pipe[1]);
    dup2(heredoc_pipe[0], STDIN_FILENO);
    close(heredoc_pipe[0]);
    return (1);
}

static int	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (cmd->in_type == 1) // Regular input redirection (<)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			return (exit_error(ERR_PERM, cmd->infile, 1, NULL));
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->in_type == 2) // Heredoc redirection (<<)
	{
		if (!handle_heredoc(cmd))
			return (0);
	}
	return (1);
}

static int	handle_output_redirection(t_command *cmd)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->out_type == 1)
		flags |= O_TRUNC;
	else if (cmd->out_type == 2)
		flags |= O_APPEND;
	if (cmd->out_type)
	{
		fd = open(cmd->outfile, flags, 0644);
		if (fd == -1)
			return (exit_error(ERR_PERM, cmd->outfile, 1, NULL));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (1);
}

int	setup_redirections(t_command *cmd)
{
	if (cmd->infile && !handle_input_redirection(cmd))
		return (0);
	if (cmd->outfile && !handle_output_redirection(cmd))
		return (0);
	return (1);
}

void	reset_redirections(int stdin_fd, int stdout_fd)
{
	// Add error checking
    if (dup2(stdin_fd, STDIN_FILENO) == -1)
        perror("dup2 stdin");
    if (dup2(stdout_fd, STDOUT_FILENO) == -1)
        perror("dup2 stdout");
    close(stdin_fd);
    close(stdout_fd);
}