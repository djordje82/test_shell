/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:15:54 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/18 12:10:40 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int validate_command_path(char *cmd, t_shell *shell)
{
    struct stat path_stat;
    char *cmd_path;

	if (!cmd || !*cmd)
		return (0);

	if (is_builtin(cmd))
		return (1);

    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &path_stat) == 0)
        {
            if (S_ISDIR(path_stat.st_mode))
            {
                print_command_error(cmd, ": Is a directory");
				g_exit_status = 126;
                return (0);
            }
            if (access(cmd, X_OK) == -1)
            {
                print_command_error(cmd, ": Permission denied");
				g_exit_status = 126;
                return (0);
            }
            return (1);
        }
        print_command_error(cmd, ": No such file or directory");
		g_exit_status = 127;
        return (0);
    }
    
    // Handle commands that need PATH lookup
    cmd_path = find_command_path(cmd, shell);
    if (!cmd_path)
    {
        print_command_error(cmd, ": command not found");
		g_exit_status = 127;
        return (0);
    }
    free(cmd_path);
    return (1);
}

static bool	handle_redirection(t_token **token, t_command *cmd)
{
	if (!parse_redirections(token, cmd))
	{
		cmd->is_valid = false;
		cleanup_cmd_list(cmd);
		return (false);
	}
	return (true);
}

static bool	process_token(t_token **token, t_command *cmd)
{
	if ((*token)->type == TOKEN_REDIR_IN || (*token)->type == TOKEN_REDIR_OUT
		|| (*token)->type == TOKEN_APPEND || (*token)->type == TOKEN_HEREDOC)
		return (handle_redirection(token, cmd));
	else if ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_SQUOTE
		|| (*token)->type == TOKEN_DQUOTE)
	{
		if (!parse_cmd_arguments(token, cmd))
		{
			cmd->is_valid = false;
			cleanup_cmd_list(cmd);
			return (false);
		}
		return (true);
	}
	*token = (*token)->next;
	return (true);
}

static void cleanup_empty_command(char **args)
{
    int i;

    i = 0;
    if (!args)
        return;
    while (args[i + 1])
    {
        args[i] = args[i + 1];
        i++;
    }
    args[i] = NULL;
}

t_command	*parse_command(t_token **token, t_shell *shell)
{
	t_command	*cmd;

	if (!token || !*token)
		return (NULL);
	cmd = create_cmd_node();
	if (!cmd)
		return (NULL);
	cmd->is_valid = true;
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		if (!process_token(token, cmd))
		{
			cleanup_cmd_list(cmd);
			return (NULL);
		}
	}
	// Handle empty commands
    if (!cmd->args)
    {
        cmd->is_valid = false;
        return (cmd);
    }
    
    if (cmd->args[0] && !cmd->args[0][0])
        cleanup_empty_command(cmd->args);

    if (!cmd->args[0])
    {
        cmd->is_valid = false;
        return (cmd);
    }

    // Command validation
    if (!validate_command_path(cmd->args[0], shell))
    {
        cmd->is_valid = false;
    }

    return (cmd);
}
