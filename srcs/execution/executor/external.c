#include "minishell.h"

int execute_external(t_command *cmd, t_shell *shell)
{
    char    *cmd_path;
    pid_t   pid;
    int     status;

    cmd_path = find_command_path(cmd->args[0], shell);
    if (!cmd_path)
        return (exit_error(ERR_CMD, cmd->args[0], 127, shell));
        
    pid = fork();
    if (pid == -1)
        return (exit_error(ERR_FORK, NULL, 1, shell));
        
    if (pid == 0)
    {
        handle_signals_child();  // Reset signal handlers for child
        if (!setup_redirections(cmd))
            exit(1);
        execve(cmd_path, cmd->args, shell->envp);
        exit_error(ERR_CMD, cmd->args[0], 127, shell);
        exit(127);
    }
    
    // Parent process
    free(cmd_path);
    waitpid(pid, &status, 0);
    
    // Handle signal-related exit status
    if (WIFSIGNALED(status))
    {
        g_exit_status = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 18);
        else if (WTERMSIG(status) == SIGINT)
            write(STDERR_FILENO, "\n", 1);
        return g_exit_status;
    }
    
    return (WEXITSTATUS(status));
}

/*int	execute_external(t_command *cmd, t_shell *shell)
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
		execve(cmd_path, cmd->args, shell->envp);
		exit_error(ERR_CMD, cmd->args[0], 127, shell);
		exit(127);
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}*/