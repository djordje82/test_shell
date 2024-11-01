#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
			!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
			!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
			!ft_strcmp(cmd, "exit"));
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (ft_echo(cmd->args));
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (ft_cd(cmd->args, shell));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(cmd->args[0], "export"))
		return (ft_export(cmd->args, shell));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (ft_unset(cmd->args, shell));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (ft_env(shell));
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (ft_exit(cmd->args, shell));
	return (1);
}

static char	*get_cmd_path(char *cmd, char **paths)
{
	char	*temp;
	char	*command;
	int		i;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		command = ft_strjoin(temp, cmd);
		free(temp);
		if (access(command, F_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}

char	*find_command_path(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	cmd_path = get_cmd_path(cmd, paths);
	free_array((void **)paths, -1);
	return (cmd_path);
}