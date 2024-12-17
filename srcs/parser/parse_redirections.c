/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:05 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 12:42:55 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_redirection(t_command *cmd, char *filename, t_token_type type)
{
	if (!filename)
		return (0);
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
	{
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = filename;
		if (type == TOKEN_REDIR_IN)
		{
			cmd->in_type = REDIR_INPUT;
		}
		else
			cmd->in_type = REDIR_HEREDOC;
	}
	else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
	{
		if (cmd->outfile)
			free(cmd->outfile);
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = filename;
		if (type == TOKEN_REDIR_OUT)
			cmd->out_type = REDIR_TRUNC;
		else
			cmd->out_type = REDIR_APPEND;
	}
	else 
	{
		free(filename);
		return (0);
	}
	return (1);
}

static int	check_output_file(char *filename, t_token_type type)
{
	int	fd;
	int flags;

	if (type != TOKEN_REDIR_OUT && type != TOKEN_APPEND)
		return (1);
	flags = O_WRONLY | O_CREAT;
	if (type == TOKEN_REDIR_OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open(filename, flags, FILE_PERMS);
	if (fd < 0)
	{
		if (errno == EACCES)
			print_file_error(filename, "Permission denied");
		else
			print_file_error(filename, strerror(errno));
		return (0);
	}
	close(fd);
	return (1);
}

/*This function is used to parse the redirections of a command.*/
int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	if (!*token || !(*token) || !cmd)
		return (0);
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		print_syntx_err("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	type = (*token)->type;
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
	{
		print_syntx_err("malloc failed", NULL);
		return (0);
	}
	if (!check_output_file(temp_file, type))
	{
		free(temp_file);
		return (0);
	}
	if (!check_output_file(temp_file, type))
	{
		free(temp_file);
		return (0);
	}
	set_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
}
