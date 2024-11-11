/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:37 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/21 17:03:10 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static void	print_pwd_error(void)
{
	ft_putstr_fd("minishell: pwd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
	ft_putendl_fd("No such file or directory", 2);
}*/

/*static char	*get_current_dir(void)
{
	char	*cwd;
	char	buffer[PATH_MAX];

	cwd = getcwd(buffer, PATH_MAX);
	if (!cwd)
		return (NULL);
	return (ft_strdup(cwd));
}*/

int ft_pwd(char **args, t_shell *shell)
{
    char current_dir[PATH_MAX];
    (void)args;  // Unused parameter

    // Get the current working directory
    if (!getcwd(current_dir, PATH_MAX))
    {
        // If getcwd fails, try to use PWD environment variable
        char *pwd = get_env_value("PWD", shell);
        if (pwd)
        {
            ft_putendl_fd(pwd, STDOUT_FILENO);
            return (0);
        }
        return (exit_error("pwd", "getcwd failed", 1, shell));
    }

    ft_putendl_fd(current_dir, STDOUT_FILENO);
    return (0);
}

/*int	ft_pwd(void)
{
	char	*cwd;

	cwd = get_current_dir();
	if (!cwd)
	{
		print_pwd_error();
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}*/
