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

static char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

char	*get_cmd_path(char *cmd, char **paths)
{
	char	*command;
	int		i;

	if (!cmd || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		command = ft_strjoin_three(paths[i], "/", cmd);
		if (!command)
			return (NULL);
		if (access(command, F_OK | X_OK) == 0)
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

	if (!cmd || !*cmd || !shell)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0) // hould be F_OK | X_OK but execute throws error???
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
