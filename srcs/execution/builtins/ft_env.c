/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:54:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/17 11:54:20 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	print_env_var(char *env_var)
{
	if (has_equals_sign(env_var))
		ft_putendl_fd(env_var, STDOUT_FILENO);
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
		print_env_var(shell->envp[i]);
		i++;
	}
	return (0);
}