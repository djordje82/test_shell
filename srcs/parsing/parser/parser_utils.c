#include "minishell.h"

static char	**add_argument(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	i = 0;
	while (args && args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (args && args[i])
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	new_args[i + 1] = NULL;
	free(args);
	return (new_args);
}

int	parse_arguments(t_token **token, t_cmd *cmd)
{
	char	**new_args;

	new_args = add_argument(cmd->args, (*token)->value);
	if (!new_args)
		return (0);
	cmd->args = new_args;
	return (1);
}

int	parse_redirections(t_token **token, t_cmd *cmd)
{
	t_token_type	type;

	type = (*token)->type;
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
		return (exit_error(ERR_SYNTAX_NEWLINE, NULL, 2, NULL), 0);
	*token = (*token)->next;
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup((*token)->value);
		cmd->in_type = (type == TOKEN_REDIRECT_IN) ? 1 : 2;
	}
	else
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = ft_strdup((*token)->value);
		cmd->out_type = (type == TOKEN_REDIRECT_OUT) ? 1 : 2;
	}
	return (cmd->infile || cmd->outfile);
}