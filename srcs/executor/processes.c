#include "minishell.h"

int	create_process(pid_t *pid, t_shell *shell)
{
	*pid = fork();
	if (*pid == -1)
		return (cleanup_and_exit("fork failed", NULL, 1, shell));
	return (1);
}

/* void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd,
		t_shell *shell)
{
	int	status;
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	setup_pipe_redirections(prev_pipe, pipe_fd);
	if (!setup_redirections(cmd))
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		status = 1;
	}
	else
	{
		setup_child_signal();
		status = execute_single_command(cmd, shell);
	}
	close(saved_stdout);
	close(saved_stdin);
	exit(status);
} */

void handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd, 
	t_shell *shell)
{
	int	status;

	setup_pipe_redirections(prev_pipe, pipe_fd);
	if (prev_pipe)
	{
		if (prev_pipe[1] != -1)
		close(prev_pipe[1]);  // Close write end of previous pipe
	}
	if (pipe_fd)
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);    // Close read end of current pipe
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
