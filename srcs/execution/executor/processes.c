#include "minishell.h"

int	create_process(pid_t *pid, t_shell *shell)
{
	*pid = fork();
	if (*pid == -1)
		return (cleanup_and_exit("fork failed", NULL, 1, shell));
	return (1);
}

void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd,
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
		setup_child_signals();
		status = execute_single_command(cmd, shell);
	}
	close(saved_stdout);
	close(saved_stdin);
	exit(status);
}

void	handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (pipe_fd[1] != -1)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
}

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	wpid;

	while ((wpid = wait(&status)) > 0)
	{
		if (WIFSIGNALED(status))
		{
			g_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 18);
			else if (WTERMSIG(status) == SIGINT)
				write(STDERR_FILENO, "\n", 1);
		}
		else if (WIFEXITED(status))
		{
			if (wpid == last_pid)
				g_exit_status = WEXITSTATUS(status);
		}
	}
}

void	handle_wait_status(int status)
{
	if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 18);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}