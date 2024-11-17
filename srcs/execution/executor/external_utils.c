/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:23 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:04:25 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_wait_status(int status)
{
	if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 18);
		else if (WTERMSIG(status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}

int	handle_wildcard_expansion(t_command *cmd, t_shell *shell)
{
	char	**expanded_args;
	int		i;
	int		expanded_count;

	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '*'))
		{
			expanded_args = expand_wildcards(cmd->args[i]);
			if (expanded_args)
			{
				expanded_count = update_command_args(cmd, expanded_args, i,
						shell);
				if (expanded_count < 0)
					return (0);
				i += expanded_count - 1;
				ft_free_array((void **)expanded_args, -1);
			}
		}
		i++;
	}
	return (1);
}

int	execute_external_command(t_command *cmd, char *cmd_path, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (exit_error(ERR_FORK, NULL, 1, shell));
	}
	if (pid == 0)
		execute_child_process(cmd, cmd_path, shell);
	free(cmd_path);
	waitpid(pid, &status, 0);
	handle_wait_status(status);
	return (g_exit_status);
}

void	setup_pipeline_execution(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signals();
	if (!setup_redirections(cmd))
		exit(1);
	execve(cmd_path, cmd->args, shell->envp);
	handle_execution_errors(cmd_path, cmd->args[0]);
}

void	handle_execution_errors(char *cmd_path, char *cmd_name)
{
	struct stat	path_stat;

	if (stat(cmd_path, &path_stat) == 0)  // If stat succeeds
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			print_command_error(cmd_name, ": Is a directory");
			exit(126);
		}
		if (access(cmd_path, X_OK) != 0)  // Check execute permission
		{
			print_command_error(cmd_name, ": Permission denied");
			exit(126);
		}
	}
	print_command_error(cmd_name, ": No such file or directory");
	exit(127);
}

/*void	handle_execution_errors(char *cmd_path, char *cmd_name)
{
	struct stat	path_stat;

	stat(cmd_path, &path_stat);
	if (S_ISDIR(path_stat.st_mode))
	{
		print_command_error(cmd_name, ": Is a directory");
		exit(126);
	}
	if (access(cmd_path, F_OK) == 0)
	{
		print_command_error(cmd_name, ": Permission denied");
		exit(126);
	}
	exit(127);
}*/

void	execute_child_process(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signals();
	if (!setup_redirections(cmd))
		exit(1);
	execve(cmd_path, cmd->args, shell->envp);
	handle_execution_errors(cmd_path, cmd->args[0]);
}

int	handle_command_not_found(t_command *cmd)
{
	if (!cmd->prev && cmd->args[0][0])  // Only check if it's first in pipeline
	{
		if (ft_strchr(cmd->args[0], '/'))
			print_command_error(cmd->args[0], ": No such file or directory");
		else
			print_command_error(cmd->args[0], ": command not found");
	}
	return (127);
}

/*int	handle_command_not_found(t_command *cmd)
{
	if (!cmd->prev && !cmd->next && cmd->args[0][0]) // Only print if not in a pipeline
	{
		if (ft_strchr(cmd->args[0], '/'))
			print_command_error(cmd->args[0], ": No such file or directory");
		else
			print_command_error(cmd->args[0], ": command not found");
	}
	return (127);
}*/