#include "minishell.h"

static int	handle_input_redirection(t_cmd *cmd)
{
	int	fd;

	if (cmd->in_type == 1)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			return (exit_error(ERR_PERM, cmd->infile, 1, NULL));
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->in_type == 2)
	{
		// TODO: Handle heredoc
	}
	return (1);
}

static int	handle_output_redirection(t_cmd *cmd)
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

int	setup_redirections(t_cmd *cmd)
{
	if (cmd->infile && !handle_input_redirection(cmd))
		return (0);
	if (cmd->outfile && !handle_output_redirection(cmd))
		return (0);
	return (1);
}

void	reset_redirections(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}