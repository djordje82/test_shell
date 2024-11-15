/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:07 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/25 13:25:40 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_flag(char *arg)
{
	int i;
    
    if (!arg || arg[0] != '-')
        return (0);
    i = 1;  // Skip the '-'
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

/*static int	ft_count_args(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}*/

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

int ft_echo(char **args, t_shell *shell)
{
    int i;
    int n_flag;
    
    (void)shell;
    //printf("Debug: Echo called with args:\n");
    if (!args[1])  // If no arguments, just print newline
    {
        ft_putchar_fd('\n', STDOUT_FILENO);
        return (0);
    }
    
    // First check all arguments for -n flags
    n_flag = 0;
    i = 1;
    if (is_n_flag(args[1]))
    {
        n_flag = 1;
        i = 2;
    }

// Print all arguments
    while (args[i])
    {
        ft_putstr_fd(args[i], STDOUT_FILENO);
        if (args[i + 1])  // If there's another argument coming
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    
    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    return (0);
}