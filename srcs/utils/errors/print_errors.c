/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:33 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 12:30:32 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quote_error(const char *result)
{
	ft_putendl_fd("minishell: syntax error: unclosed quotes", STDERR_FILENO);
	free((char *)result);
	g_exit_status = 2;
	return (NULL);
}

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

void	print_command_error(char *cmd, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}
