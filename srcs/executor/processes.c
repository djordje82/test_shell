#include "minishell.h"

int	create_process(pid_t *pid, t_shell *shell)
{
	*pid = fork();
	if (*pid == -1)
		return (cleanup_and_exit("fork failed", NULL, 1, shell));
	return (1);
}
void handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd, 
	t_shell *shell)
{
	int	status;

	setup_pipe_redirections(prev_pipe, pipe_fd);
	if (prev_pipe)
	{
		if (prev_pipe[1] != -1)
		close(prev_pipe[1]);
	}
	if (pipe_fd)
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
	}
	if (!setup_redirections(cmd))
		exit(1);
	init_signals(false);
	status = execute_single_command(cmd, shell);
	exit(status);
}

void handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	close_pipe(prev_pipe, NULL);  // Close previous pipe ends
	if (pipe_fd[1] != -1)  // If there's a next command
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
	else
		close_pipe(NULL, pipe_fd);
}
