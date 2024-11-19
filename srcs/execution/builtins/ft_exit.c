/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:54:29 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/24 15:38:19 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to print the error message for the exit command.*/
static void	print_exit_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	if (!args || !shell)
		return (EXIT_ERROR_NUMERIC);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
		exit_code = g_exit_status;
	else if (!ft_isnumber(args[1]))
	{
		print_exit_error(args[1]);
		exit_code = EXIT_ERROR_NUMERIC;
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		g_exit_status = EXIT_ERROR_ARGS;
		return (EXIT_ERROR_ARGS);
	}
	else
		exit_code = (ft_atoi(args[1]) % 256);
	cleanup_shell_data(shell);
	exit(exit_code);
}

/*This function is used to exit the shell.*/
/*int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
		exit_code = g_exit_status;
	else if (!(ft_isnumber(args[1])))
	{
		print_exit_error(args[1]);
		exit_code = 2;
	}
	else if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		g_exit_status = 1;
		return (1);
	}
	else
		//exit_code = (unsigned char)ft_atoll(args[1]); // Change back to atoi with bitwise operation
		exit_code = (ft_atoi(args[1]) % 256);
	cleanup_shell_data(shell);
	exit(exit_code);
	return (exit_code);
}*/
