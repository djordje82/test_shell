/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:15 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:10:02 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_msg(char *err_msg, char *src)
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

int	cleanup_and_exit(char *err_msg, char *src, int err_code, t_shell *shell)
{
	print_error_msg(err_msg, src);
	if (shell)
	{
		shell->exit_status = err_code;
		cleanup_shell_data(shell);
	}
	exit(err_code);
}
