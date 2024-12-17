/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:51:35 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 14:09:18 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_dir(t_shell *shell)
{
	char	*home;
	char	*result;

	home = get_env_value("HOME", shell);
	if (!home)
		return (ft_strdup(getenv("HOME")));
	result = ft_strdup(home);
	return (result);
}

int	update_pwd_vars(t_shell *shell)
{
	char	*old_pwd;
	char	current_dir[PATH_MAX];

	old_pwd = get_env_value("PWD", shell);
	if (old_pwd)
	{
		if (update_env_value("OLDPWD", old_pwd, shell) != 0)
			return (1);
	}
	if (!getcwd(current_dir, PATH_MAX))
		return (1);
	if (update_env_value("PWD", current_dir, shell) != 0)
		return (1);
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

static void	print_cd_error(char *path)
{
	if (errno == ENOENT)
		print_file_error(path, "No such file or directory");
	else if (errno == ENOTDIR)
		print_file_error(path, "Not a directory");
	else if (errno == EACCES)
		print_file_error(path, "Permission denied");
	else
		print_file_error(path, "Error changing directory");
}

int	ft_cd(char **args, t_shell *shell)
{
	int		ret;
	char	*path;

	if (ft_count_args(args) > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (!args[1])
		path = get_home_dir(shell);
	else
		path = expand_path(args[1], shell);
	if (!path)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (1);
	}
	if (chdir(path) == -1)
	{
		print_cd_error(args[1]);
		ret = 1;
	}
	else
		ret = update_pwd_vars(shell);
	return (ret);
}
