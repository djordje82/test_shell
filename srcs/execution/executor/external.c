#include "minishell.h"

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*cmd_path;
	pid_t	pid;
	int		status;

	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
		return (exit_error(ERR_CMD, cmd->args[0], 127, shell));
	pid = fork();
	if (pid == -1)
		return (exit_error(ERR_FORK, NULL, 1, shell));
	if (pid == 0)
	{
		handle_signals_child();
		if (!setup_redirections(cmd))
			exit(1);
		execve(cmd_path, cmd->args, shell->env);
		exit_error(ERR_CMD, cmd->args[0], 127, shell);
		exit(127);
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}