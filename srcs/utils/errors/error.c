/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:33 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:00:38 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// New function for syntax errors
int	syntax_error(char *err_msg, char *src)
{
	if (src)
		ft_putstr_fd(src, STDERR_FILENO);
	if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	g_exit_status = 2; // Set syntax error exit status
	return (0);
}

void	print_command_error(char *cmd, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
}

int	exit_error(char *err_msg, char *src, int err_code, t_shell *shell)
{
	char *result;

	if (src)
	{
		result = ft_strjoin(src, err_msg);
		if (!result)
		{
			ft_putendl_fd(ERR_MEM, STDERR_FILENO);
			exit(-1);
		}
		ft_putendl_fd(result, STDERR_FILENO);
		free(result);
	}
	else if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	if (shell)
	{
		if (shell->cmnd_lst)
			free_cmd_list(shell->cmnd_lst);
		if (shell->envp)
			ft_free_array((void **)shell->envp, -1);
		if (shell->tokens)
			free_tokens(shell->tokens);
		if (shell->pipe)
			ft_free_array((void **)shell->pipe, shell->n_cmnds);
		shell->exit_status = err_code;
	}
	rl_clear_history();
	exit(err_code);
}