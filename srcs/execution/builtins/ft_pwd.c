/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:37 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:53:50 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args, t_shell *shell)
{
	char current_dir[PATH_MAX];
	(void)args; // Unused parameter

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