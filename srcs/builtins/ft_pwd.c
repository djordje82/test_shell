/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:37 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/25 14:07:04 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to print the current working directory.*/
int	ft_pwd(char **args, t_shell *shell)
{
	char	current_dir[PATH_MAX];
	char	*pwd;

	(void)args;
	if (!getcwd(current_dir, PATH_MAX))
	{
		pwd = get_env_value("PWD", shell);
		if (pwd)
		{
			ft_putendl_fd(pwd, STDOUT_FILENO);
			return (0);
		}
		return (cleanup_and_exit("pwd", "getcwd failed", 1, shell));
	}
	ft_putendl_fd(current_dir, STDOUT_FILENO);
	return (0);
}
