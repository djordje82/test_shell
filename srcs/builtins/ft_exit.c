/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:54:29 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/11 17:57:49 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_status;

	if (!args || !shell)
		return (EXIT_ERROR_NUMERIC);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
		exit_status = g_exit_status;
	else if (!ft_isnumber(args[1]))
	{
		print_exit_error(args[1]);
		exit_status = EXIT_ERROR_NUMERIC;
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		g_exit_status = EXIT_ERROR_ARGS;
		return (EXIT_ERROR_ARGS);
	}
	else
		exit_status = (ft_atoi(args[1]) & 0xFF);
	cleanup_shell_data(shell);
	exit(exit_status);
}
