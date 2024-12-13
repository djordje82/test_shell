/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:05 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/12 17:10:28 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_redirection(t_command *cmd, char *filename, t_token_type type)
{
	if (!filename)
		return ;
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = filename;
		if (type == TOKEN_REDIR_IN)
		{
			cmd->in_type = REDIR_INPUT;
		}
		else
		{
			cmd->in_type = REDIR_HEREDOC;
		}
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
/* int	parse_redirections(t_token **token, t_command *cmd)
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
	{
		print_syntx_err("malloc failed", NULL);
		return (0);
	}
	set_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
} */

static int check_output_file(char *filename, t_token_type type)
{
    int flags;
    int fd;

    if (type != TOKEN_REDIR_OUT && type != TOKEN_APPEND)
        return (1);
        
    flags = O_WRONLY | O_CREAT;
    if (type == TOKEN_REDIR_OUT)
        flags |= O_TRUNC;
    else
        flags |= O_APPEND;

    fd = open(filename, flags, FILE_PERMS);
    if (fd == -1)
    {
        if (errno == ENOENT)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(filename, STDERR_FILENO);
            ft_putendl_fd(": No such file or directory", STDERR_FILENO);
            return (0);
        }
    }
    if (fd != -1)
        close(fd);
    return (1);
}

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
	{
		print_syntx_err("malloc failed", NULL);
		return (0);
	}
    
    // Check output files first
    if (!check_output_file(temp_file, type))
    {
        free(temp_file);
        return (0);
    }
    
	set_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
}
