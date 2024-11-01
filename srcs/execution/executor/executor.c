#include "minishell.h"

static int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	int	stdin_fd;
	int	stdout_fd;
	int	status;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd))
		return (0);
	if (is_builtin(cmd->args[0]))
		status = execute_builtin(cmd, shell);
	else
		status = execute_external(cmd, shell);
	reset_redirections(stdin_fd, stdout_fd);
	return (status);
}

static int	execute_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (exit_error(ERR_PIPE, NULL, 1, shell));
	pid = fork();
	if (pid == -1)
		return (exit_error(ERR_FORK, NULL, 1, shell));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		status = execute_single_command(cmd, shell);
		exit(status);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (pid);
}

static void	wait_for_children(pid_t last_pid)
{
	int	status;

	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}

int	execute_commands(t_shell *shell)
{
	t_cmd	*current;
	pid_t	last_pid;
	int		stdin_fd;

	if (!shell->cmd_list)
		return (1);
	stdin_fd = dup(STDIN_FILENO);
	current = shell->cmd_list;
	while (current && current->next)
	{
		if (execute_pipeline(current, shell) == -1)
			return (0);
		current = current->next;
	}
	last_pid = execute_single_command(current, shell);
	dup2(stdin_fd, STDIN_FILENO);
	close(stdin_fd);
	if (last_pid > 0)
		wait_for_children(last_pid);
	return (1);
}