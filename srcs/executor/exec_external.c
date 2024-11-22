/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:39 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:42 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function executes a single external command. It forks a child process, executes the command, and handles errors.*/
int	execute_external_single_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (cleanup_and_exit(ERR_FORK, NULL, 1, shell));
	}
	if (pid == 0)
		execute_external_child(cmd, cmd_path, shell);
	free(cmd_path);
	waitpid(pid, &status, 0);
	handle_wait_status(status);
	return (g_exit_status);
}

/*This function executes an external command. It handles wildcard expansion, checks for command not found, and sets up pipeline execution if necessary.*/
int	handle_external_cmd(t_command *cmd, t_shell *shell)
{
	char	*cmd_path;

//	if (!handle_wildcard_expansion(cmd, shell))
//		return (1);
	signal(SIGINT, SIG_IGN);
	if (!cmd->args || !cmd->args[0] || !cmd->args[0][0])
		return (0);
	cmd_path = find_command_path(cmd->args[0], shell);
	if (!cmd_path)
		return (print_command_not_found(cmd));
	if (cmd->prev || cmd->next)
	{
		execute_pipeline_cmd(cmd, cmd_path, shell);
		free(cmd_path);
		return (0);
	}
	return (execute_external_single_cmd(cmd, cmd_path, shell));
}
