#include "minishell.h"

int is_builtin(char *cmd)
{
    return (ft_strncmp(cmd, "cd", 3) == 0 ||
            ft_strncmp(cmd, "pwd", 4) == 0 ||
            ft_strncmp(cmd, "echo", 5) == 0 ||
            ft_strncmp(cmd, "export", 7) == 0 ||
            ft_strncmp(cmd, "unset", 6) == 0 ||
            ft_strncmp(cmd, "env", 4) == 0 ||
            ft_strncmp(cmd, "exit", 5) == 0);
}

int execute_builtin(t_command *cmd, t_shell *shell)
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

char	*get_cmd_path(char *cmd, char **paths)
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