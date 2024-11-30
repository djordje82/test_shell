/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:51:35 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/30 14:39:39 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to get the home directory of the user.*/
char	*get_home_dir(t_shell *shell)
{
	char	*home;

	home = get_env_value("HOME", shell);
	if (!home)
		return (ft_strdup(getenv("HOME")));
	return (ft_strdup(home));
}

/*This function is used to update the PWD and OLDPWD environment variables.*/
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

/*This function is used to expand the path of the current directory.*/
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

/*This function is used to print the error message for the cd command.*/
static void	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (errno == ENOENT)
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
	else if (errno == EACCES)
		ft_putendl_fd("Permission denied", STDERR_FILENO);
	else if (errno == ENOTDIR)
		ft_putendl_fd("Not a directory", STDERR_FILENO);
	else
		ft_putendl_fd("Error changing directory", STDERR_FILENO);
}

/*This function is used to change the current directory of the user.*/
int	ft_cd(char **args, t_shell *shell)
{
	int	ret;

	if (ft_count_args(args) > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (!args[1])
	{
		ft_putendl_fd("minishell: cd: path required", STDERR_FILENO);
		return (1);
	}
	if (chdir(args[1]) == -1)
	{
		print_cd_error(args[1]);
		ret = 1;
	}
	else
		ret = update_pwd_vars(shell);
	return (ret);
}
