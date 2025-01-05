/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:39 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/05 10:42:45 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_external_child(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
	{
		perror("redirection failed");
		exit(1);
	}
	execve(cmd_path, cmd->args, shell->envp);
	handle_command_errors(cmd_path, cmd->args[0]);
	exit(g_exit_status);
}

int	execute_external_single_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	pid_t	pids;
	int		status;

	pids = fork();
	if (pids == -1)
	{
		free(cmd_path);
		return (cleanup_and_exit(ERR_FORK, NULL, 1, shell));
	}
	if (pids == 0)
		execute_external_child(cmd, cmd_path, shell);
	free(cmd_path);
	waitpid(pids, &status, 0);
	handle_wait_status(status);
	return (g_exit_status);
}

int	handle_external_cmd(t_command *cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!cmd->args || !cmd->args[0] || !cmd->args[0][0])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		g_exit_status = 127;
		return (127);
	}
	signal(SIGINT, SIG_IGN);
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
		return (print_command_not_found(cmd));
	if (cmd->prev || cmd->next)
	{
		execute_pipeline_cmd(cmd, cmd_path, shell);
		free(cmd_path);
		return (0);
	}
	g_exit_status = execute_external_single_cmd(cmd, cmd_path, shell);
	signal(SIGINT, SIG_DFL);
	return (g_exit_status);
}
