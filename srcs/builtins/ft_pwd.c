/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:37 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/03 16:47:54 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to print the current working directory.*/
int	ft_pwd(char **args, t_shell *shell)
{
	char	*current_dir;
	char	*pwd;

	(void)args;
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		pwd = get_env_value("PWD", shell);
		if (pwd)
		{
			ft_putendl_fd(pwd, STDOUT_FILENO);
			return (0);
		}
		ft_putendl_fd("minishell: pwd: error pwd failed", STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(current_dir, STDOUT_FILENO);
	free(current_dir);
	return (0);
}
