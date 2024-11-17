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

int ft_exit(char **args, t_shell *shell)
{
    int exit_code;
    
    //printf("Debug: Executing exit command\n");
    
    // Print "exit" to stdout (not stderr) as per bash behavior
    ft_putendl_fd("exit", STDOUT_FILENO);
    
    if (!args[1])
    {
        //printf("Debug: No arguments, using current exit status: %d\n", g_exit_status);
        exit_code = g_exit_status;
    }
    else if (!(ft_isnumber(args[1])))
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
    cleanup_shell_data(shell);
    exit(exit_code);
    
    return exit_code;  // Never reached
}