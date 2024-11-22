#include "minishell.h"

/*This function checks if a command is a builtin command.*/
int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

/*This function routes a builtin command to the appropriate handler function.*/
int	handle_builtin_cmd(t_command *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd->args, shell));
	return (1);
}

/*This function executes a single builtin command. It handles redirections, command execution, and cleanup.*/
int	execute_single_builtin(t_command *cmd, t_shell *shell)
{
	int	status;
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd))
	{
		restore_std_fds(stdin_fd, stdout_fd);
		return (g_exit_status);
	}
	status = handle_builtin_cmd(cmd, shell);
	restore_std_fds(stdin_fd, stdout_fd);
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
	{
		shell->running = false;
		shell->exit_status = status;
	}
	return (status);
}