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

/*This function is used to print the environment variables.*/
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