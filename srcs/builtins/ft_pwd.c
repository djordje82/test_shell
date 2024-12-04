/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:37 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 16:02:16 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
