/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:10:01 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 17:26:02 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

int	handle_builtin_cmd(t_command *cmd, t_shell *shell)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(cmd->args, shell));
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd->args, shell));
	return (1);
}

int	execute_single_builtin(t_command *cmd, t_shell *shell)
{
	int	status;
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (stdin_fd == -1 || stdout_fd == -1)
	{
		perror("dup fail");
		return (1);
	}
	if (!setup_redirections(cmd))
	{
		restore_std_fds(stdin_fd, stdout_fd);
		return (g_exit_status);
	}
	status = handle_builtin_cmd(cmd, shell);
	restore_std_fds(stdin_fd, stdout_fd);
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
	{
		shell->running = false;
		shell->exit_status = status;
	}
	return (status);
}
