/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:05 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/09 13:38:25 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int validate_input_file(char *filename)
{
    if (access(filename, F_OK) != 0)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(filename, STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        g_exit_status = 1;
        return (0);
    }
    if (access(filename, R_OK) != 0)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(filename, STDERR_FILENO);
        ft_putendl_fd(": Permission denied", STDERR_FILENO);
        g_exit_status = 1;
        return (0);
    }
    return (1);
}

static int validate_output_path(char *filename)
{
    char *dir_path;
    char *last_slash;
    //int ret;

    dir_path = ft_strdup(filename);
    if (!dir_path)
        return (0);
    last_slash = ft_strrchr(dir_path, '/');
    if (last_slash)
    {
        *last_slash = '\0';
        if (access(dir_path, W_OK) != 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(filename, STDERR_FILENO);
            ft_putendl_fd(": Permission denied", STDERR_FILENO);
            free(dir_path);
            g_exit_status = 1;
            return (0);
        }
    }
    free(dir_path);
    return (1);
}

static void	set_redirection(t_command *cmd, char *filename, t_token_type type)
{
	if (!filename)
		return ;
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
	{
		free(cmd->infile);
		cmd->infile = filename;
		if (type == TOKEN_REDIR_IN)
			cmd->in_type = REDIR_INPUT;
		else
			cmd->in_type = REDIR_HEREDOC;
	}
	else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
	{
		free(cmd->outfile);
		cmd->outfile = filename;
		if (type == TOKEN_REDIR_OUT)
			cmd->out_type = REDIR_TRUNC;
		else
			cmd->out_type = REDIR_APPEND;
	}
	else 
		free(filename);
}

/*This function is used to parse the redirections of a command.*/
int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	if (!*token || !(*token) || !cmd)
		return (0);
	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		print_syntx_err("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
        return (0);
	// Set cmd->is_valid based on file validation
    if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
    {
        if (type == TOKEN_REDIR_IN && !validate_input_file(temp_file))
            cmd->is_valid = false;
    }
    else if ((type == TOKEN_REDIR_OUT || type == TOKEN_APPEND) 
             && !validate_output_path(temp_file))
			 {
				cmd->is_valid = false;
			 }

	set_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
}
