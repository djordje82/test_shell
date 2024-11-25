/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:07 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/25 14:11:35 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to check if the argument is a valid -n flag.*/
int	is_valid_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}


/*This function is used to print the arguments.*/
void	print_args(char **args, int start, int n_flag)
{
	int	i;

	i = start;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

/*This function is used to print the arguments.*/
int	ft_echo(char **args, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	n_flag = 0;
	if (!args[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (g_exit_status);
	}
	i = 1;
	if (is_valid_n_flag(args[1]))
	{
		n_flag = 1;
		i = 2;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (g_exit_status);
}
