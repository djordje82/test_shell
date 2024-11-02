/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:51:35 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/21 13:05:29 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_dir(t_shell *shell)
{
	char	*home;

	home = get_env_value("HOME", shell);
	if (!home)
		return (ft_strdup(getenv("HOME")));
	return (ft_strdup(home));
}

int	update_pwd_vars(t_shell *shell)
{
	char	*old_pwd;
	char	*new_pwd;
	char	current_dir[PATH_MAX];

	old_pwd = get_env_value("PWD", shell);
	if (old_pwd)
	{
		if (update_env_value("OLDPWD", old_pwd, shell) != 0)
			return (1);
	}
	if (!getcwd(current_dir, PATH_MAX))
		return (1);
	new_pwd = ft_strdup(current_dir);
	if (!new_pwd)
		return (1);
	if (update_env_value("PWD", new_pwd, shell) != 0)
	{
		free(new_pwd);
		return (1);
	}
	free(new_pwd);
	return (0);
}

char	*expand_path(char *path, t_shell *shell)
{
	char	*expanded;
	char	*home;

	if (!path || !*path)
	{
		home = get_home_dir(shell);
		if (!home)
			return (NULL);
		return (home);
	}
	if (path[0] == '~')
	{
		home = get_home_dir(shell);
		if (!home)
			return (NULL);
		expanded = ft_strjoin(home, path + 1);
		free(home);
		return (expanded);
	}
	return (ft_strdup(path));
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	int		ret;

	if (!args[1])
		path = expand_path(NULL, shell);
	else
		path = expand_path(args[1], shell);
	if (!path)
		return (exit_error("minishell: cd: HOME not set", NULL, 1, shell));
	ret = chdir(path);
	if (ret != 0)
	{
		free(path);
		return (exit_error("minishell: cd: No such file or directory",
				args[1], 1, shell));
	}
	free(path);
	if (update_pwd_vars(shell) != 0)
		return (exit_error("minishell: cd: error updating PWD",
				NULL, 1, shell));
	return (0);
}