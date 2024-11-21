/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:05:32 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:47:37 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interactive_signal_handler(int signum)
{
	g_exit_status = 1;
	if (signum == SIGINT)
	{
		g_exit_status = 130; // 128 + SIGINT(2)
		write(STDERR_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		g_exit_status = 131; // 128 + SIGQUIT(3)
		write(STDERR_FILENO, "Quit (core dumped)\n", 18);
	}
}

void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	shell->exit_status = CTRL_D_EXIT_CODE;
	shell->running = false;
}
