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

int	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int ft_exit(char **args, t_shell *shell)
{
    int exit_code;
    
    //printf("Debug: Executing exit command\n");
    
    // Print "exit" to stderr as per bash behavior
    ft_putendl_fd("exit", STDERR_FILENO);
    
    if (!args[1])
    {
        //printf("Debug: No arguments, using current exit status: %d\n", g_exit_status);
        exit_code = g_exit_status;
    }
    else if (!is_numeric_arg(args[1]))
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(args[1], STDERR_FILENO);
        ft_putendl_fd(": numeric argument required", STDERR_FILENO);
        exit_code = 2;
    }
    else if (args[2])
    {
        ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
        g_exit_status = 1;  // Set error status
        return 1;  // Return without exiting
    }
    else
    {
        exit_code = (unsigned char)ft_atoll(args[1]);
    }
    
    // Only reach here if we should actually exit
    //printf("Debug: Exiting with code: %d\n", exit_code);
    cleanup_shell(shell);
    exit(exit_code);
    
    return exit_code;  // Never reached
}