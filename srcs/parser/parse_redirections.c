#include "minishell.h"

/*This function is used to set the input redirection of a command.*/
static void	set_input_redirection(t_command *cmd, char *filename, t_token_type type)
{
	free(cmd->infile);
	cmd->infile = filename;
	if (type == TOKEN_RDRCT_IN)
		cmd->in_type = 1;
	else
		cmd->in_type = 2;
}

/*This function is used to set the output redirection of a command.*/
static void	set_output_redirection(t_command *cmd, char *filename, t_token_type type)
{
	free(cmd->outfile);
	cmd->outfile = filename;
	if (type == TOKEN_RDRCT_OUT)
		cmd->out_type = 1;
	else
		cmd->out_type = 2;
}

/*This function is used to parse the redirections of a command.*/
int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;
	char			*temp_file;

	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
	{
		print_syntx_err("syntax error near unexpected token `newline'", NULL);
		return (0);
	}
	*token = (*token)->next;
	temp_file = ft_strdup((*token)->value);
	if (!temp_file)
		return (cleanup_and_exit(ERR_MEM, NULL, 1, NULL));
	if (type == TOKEN_RDRCT_IN || type == TOKEN_HEREDOC)
		set_input_redirection(cmd, temp_file, type);
	else
		set_output_redirection(cmd, temp_file, type);
	*token = (*token)->next;
	return (1);
}
