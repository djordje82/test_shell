/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:33 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 14:09:46 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function prints a syntax error message. If a source string is provided,
	it appends the error message to the source string and prints the result.*/
//#########/// use to replace all ft_putendl_fd and ft_putstr_fd(src, STDERR_FILENO); with ft_putstr_fd(src, STDERR_FILENO); in all files

int	print_syntx_err(char *err_msg, char *src)
{
	if (src)
	{
		ft_putstr_fd("minishell:", STDERR_FILENO);
		ft_putstr_fd(src, STDERR_FILENO);
	}
	if (err_msg)
	{
		if (src)
			ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(err_msg, STDERR_FILENO);
	}
	g_exit_status = 2;
	return (0);
}

void	print_file_error(const char *filename, const char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error_msg && *error_msg == '\n')
		ft_putendl_fd(error_msg + 1, STDERR_FILENO);
	else
		ft_putendl_fd(error_msg, STDERR_FILENO);
}

/*This function prints a command error message. 
It prepends "minishell: " to the command and 
appends the error message to it.*/
void	print_command_error(char *cmd, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}
