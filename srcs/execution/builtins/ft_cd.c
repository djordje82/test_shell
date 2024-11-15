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
	char *old_pwd;
    char current_dir[PATH_MAX];

    //printf("Debug: Entering update_pwd_vars\n");
    
    // Get and update OLDPWD
    old_pwd = get_env_value("PWD", shell);
    if (old_pwd)
    {
        //printf("Debug: Found old PWD: %s\n", old_pwd);
        if (update_env_value("OLDPWD", old_pwd, shell) != 0)
        {
            //printf("Debug: Failed to update OLDPWD\n");
            return (1);
        }
        //printf("Debug: Successfully updated OLDPWD\n");
    }

    // Get and update PWD
    if (!getcwd(current_dir, PATH_MAX))
    {
        //printf("Debug: getcwd failed\n");
        return (1);
    }
    //printf("Debug: Current directory: %s\n", current_dir);

    if (update_env_value("PWD", current_dir, shell) != 0)
    {
        //printf("Debug: Failed to update PWD\n");
        return (1);
    }
    //printf("Debug: Successfully updated PWD\n");
    
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

int ft_cd(char **args, t_shell *shell)
{
    int ret;

    // Check argument count
    if (ft_count_args(args) > 2)
    {
        ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
        return (1);
    }

    // If no argument is provided, return error
    if (!args[1])
    {
        ft_putendl_fd("minishell: cd: path required", STDERR_FILENO);
        return (1);
    }
    
    // Try to change directory
    if (chdir(args[1]) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(args[1], STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        
        if (errno == ENOENT)
            ft_putendl_fd("No such file or directory", STDERR_FILENO);
        else if (errno == EACCES)
            ft_putendl_fd("Permission denied", STDERR_FILENO);
        else if (errno == ENOTDIR)
            ft_putendl_fd("Not a directory", STDERR_FILENO);
        else
            ft_putendl_fd("Error changing directory", STDERR_FILENO);

        ret = 1;
    }
    else
        ret = update_pwd_vars(shell);
    
    return (ret);
}