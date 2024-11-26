/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:07 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cmd_path(char *cmd, char **paths)
{
	char	*temp;
	char	*command;
	int		i;

	if (!cmd || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		command = ft_strjoin(temp, cmd);
		if (!command)
		{
			free(temp);
			return (NULL);
		}
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}
/*char	*get_cmd_path(char *cmd, char **paths)
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
}*/

char	*find_command_path(char *cmd, t_shell *shell)
{
	char *path_env;
	char **paths;
	char *cmd_path;

	if (!cmd || !*cmd || !shell)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	cmd_path = get_cmd_path(cmd, paths);
	ft_free_array((void **)paths, -1);
	return (cmd_path);
}
/*char	*find_command_path(char *cmd, t_shell *shell)
{
	char *path_env;
	char **paths;
	char *cmd_path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value("PATH", shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	cmd_path = get_cmd_path(cmd, paths);
	ft_free_array((void **)paths, -1);
	return (cmd_path);
}*/
