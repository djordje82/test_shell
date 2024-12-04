/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:54:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:56:38 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_env_args(char **args)
{
	if (args[1])
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd("': too many arguments", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	ft_env(char **args, t_shell *shell)
{
	int	i;

	if (!check_env_args(args))
		return (127);
	if (!shell->envp)
	{
		ft_putendl_fd("env: environment not available", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
			ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
