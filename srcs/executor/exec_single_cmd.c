#include "minishell.h"

/*This function cleans up an empty command. It shifts the arguments to remove the empty command.*/
static void	cleanup_empty_command(char **args)
{
	int	i;

	i = 0;
	while (args[i + 1])
	{
		args[i] = args[i + 1];
		i++;
	}
	args[i] = NULL;
}

/*This function executes a command based on its type. It handles built-in commands, external commands, and exits the shell if the command is "exit".*/
static int	execute_command_type(t_command *cmd, t_shell *shell,
	int stdin_fd, int stdout_fd)
{
	int	status;

	if (is_builtin(cmd->args[0]))
	{
		status = handle_builtin_cmd(cmd, shell);
		if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
		{
			restore_std_fds(stdin_fd, stdout_fd);
			exit(status);
		}
	}
	else
		status = handle_external_cmd(cmd, shell);
	return (status);
}

/*This function executes a single command. It handles redirections, command execution, and cleanup.*/
int	execute_single_command(t_command *cmd, t_shell *shell)
{
	int	stdin_fd;
	int	stdout_fd;
	int	status;

	if (!cmd->args)
		return (0);
	if (cmd->args[0] && !cmd->args[0][0])
		cleanup_empty_command(cmd->args);
	if (!cmd->args[0])
		return (0);
	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!setup_redirections(cmd))
	{
		restore_std_fds(stdin_fd, stdout_fd);
		return (g_exit_status);
	}
	status = execute_command_type(cmd, shell, stdin_fd, stdout_fd);
	restore_std_fds(stdin_fd, stdout_fd);
	return (status);
}