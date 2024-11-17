#include "minishell.h"

/*int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		syntax_error("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
		return (exit_error(ERR_MEM, NULL, 1, NULL));
	if (type == TOKEN_RDRCT_IN || type == TOKEN_HEREDOC)
	{
		if (type == TOKEN_RDRCT_IN && access(temp_file, F_OK) == -1)
		{
			g_exit_status = 1;
			exit_error(ERR_NOFILE, temp_file, 1, NULL);
			free(temp_file);
			*token = (*token)->next;
			return (1);  // Continue parsing but maintain error state
		}
		free(cmd->infile);
		cmd->infile = temp_file;
		cmd->in_type = (type == TOKEN_RDRCT_IN) ? 1 : 2;
	}
	else
	{
		free(cmd->outfile);
		cmd->outfile = temp_file;
		cmd->out_type = (type == TOKEN_RDRCT_OUT) ? 1 : 2;
	}
	*token = (*token)->next;
	return (1);
}*/
int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	type = (*token)->type;

	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		syntax_error("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
		return (exit_error(ERR_MEM, NULL, 1, NULL));

	if (type == TOKEN_RDRCT_IN || type == TOKEN_HEREDOC)
	{
		free(cmd->infile);
		cmd->infile = temp_file;
		cmd->in_type = (type == TOKEN_RDRCT_IN) ? 1 : 2;
	}
	else
	{
		free(cmd->outfile);
		cmd->outfile = temp_file;
		cmd->out_type = (type == TOKEN_RDRCT_OUT) ? 1 : 2;
	}
	*token = (*token)->next;
	return (1);
}