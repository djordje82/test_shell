/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:33 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:07:59 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function prints a syntax error message. If a source string is provided, it appends the error message to the source string and prints the result.*/
int	print_syntx_err(char *err_msg, char *src)
{
	if (src)
		ft_putstr_fd(src, STDERR_FILENO);
	if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	g_exit_status = 2;
	return (0);
}

void print_file_error(const char *filename, const char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}

/*This function prints a command error message. It prepends "minishell: " to the command and appends the error message to it.*/
void	print_command_error(char *cmd, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

/*This function prints a redirection error message. It prepends the file name to the error message and prints the result.*/
void	print_redir_error(char *msg, char *file)
{
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

/*This function prints an error message. If a source string is provided, it appends the error message to the source string and prints the result.*/
/* void	print_error(char *err_msg, char *src)
{
	char	*result;

	if (src)
	{
		result = ft_strjoin(src, err_msg);
		if (!result)
		{
			ft_putendl_fd(ERR_MEM, STDERR_FILENO);
			//exit(-1);
		}
		ft_putendl_fd(result, STDERR_FILENO);
		free(result);
	}
	else if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
} */

void print_error(char *err_msg, char *src)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
    
    if (src)
    {
        ft_putstr_fd(src, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
    }
    
    if (err_msg)
        ft_putendl_fd(err_msg, STDERR_FILENO);
}
