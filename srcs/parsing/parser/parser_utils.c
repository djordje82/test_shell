#include "minishell.h"

static char	**add_argument(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	if (!new_arg)
		return (args);
	//printf("Debug: add_argument called with new_arg: '%s'\n", new_arg);
	i = 0;
	while (args && args[i])
		i++;
	//printf("Debug: Current arg count: %d\n", i);
	
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
    {
        if (args)
            free_array((void **)args, -1);
        return (NULL);
    }

	i = 0;
	while (args && args[i])
	{
		new_args[i] = ft_strdup(args[i]);
		if (!new_args[i])
		{
			//printf("Debug: Failed to duplicate arg %d\n", i);
			free_array((void **)new_args, i);
			if (args)
				free_array((void **)args, -1);
			return (NULL);
		}
		//printf("Debug: Copied arg[%d]: '%s'\n", i, new_args[i]);
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		//printf("Debug: Failed to duplicate new_arg\n");
		free_array((void **)new_args, i);
		if (args)
			free_array((void **)args, -1);
		return (NULL);
	}
	//printf("Debug: Added new arg[%d]: '%s'\n", i, new_args[i]);
	new_args[i + 1] = NULL;
	
	if (args)
	{
		//printf("Debug: Freeing old args array\n");
		free_array((void **)args, -1);
	}
	//printf("Debug: Successfully created new args array\n");
	return (new_args);
}

/*static char	**add_argument(char **args, char *new_arg)
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
}*/

static char *process_token_value(char *value, t_token_type type)
{
    char *result;
    
	//printf("Debug: Processing token value: '%s' of type %d\n", value, type);
	(void)type;
    /*if (type == TOKEN_QUOTE || type == TOKEN_DQUOTE)
    {
        // Remove surrounding quotes
        //result = ft_substr(value, 1, ft_strlen(value) - 2);
		result = ft_strdup(value + 1);
    }
    else
    {
        result = ft_strdup(value);
    }*/
    result = ft_strdup(value);
    //printf("Debug: Processed result: '%s'\n", result);

    return result;
}

int	parse_arguments(t_token **token, t_command *cmd)
{
	char    **new_args;
    char    *processed_value;
    
    //printf("Debug: Parsing argument token: '%s' of type %d\n", (*token)->value, (*token)->type);
    
    processed_value = process_token_value((*token)->value, (*token)->type);
    if (!processed_value)
        return (0);
    
    //printf("Debug: Adding processed value: '%s'\n", processed_value);
    
    new_args = add_argument(cmd->args, processed_value);
    free(processed_value);
    
    if (!new_args)
        return (0);
    
    cmd->args = new_args;
    *token = (*token)->next;
    return (1);
}

int parse_redirections(t_token **token, t_command *cmd)
{
    t_token_type type;
    char *temp_file = NULL;

    type = (*token)->type;
    if ((type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC) && cmd->in_type != 0)
    {
        syntax_error("syntax error: multiple input redirections", NULL);
        return 0;
    }
    if ((type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND) && cmd->out_type != 0)
    {
        syntax_error("syntax error: multiple output redirections", NULL);
        return 0;
    }
    if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
    {
        syntax_error("syntax error near unexpected token `newline'", NULL);
        return 0;
    }
    
    *token = (*token)->next;
    temp_file = ft_strdup((*token)->value);
    if (!temp_file)
    {
        exit_error(ERR_MEM, NULL, 1, NULL);
        return 0;
    }

    if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
    {
        free(cmd->infile);
        cmd->infile = temp_file;
        cmd->in_type = (type == TOKEN_REDIRECT_IN) ? 1 : 2;
    }
    else
    {
        free(cmd->outfile);
        cmd->outfile = temp_file;
        cmd->out_type = (type == TOKEN_REDIRECT_OUT) ? 1 : 2;
    }
    return 1;
}

/*int	parse_redirections(t_token **token, t_command *cmd)
{
	t_token_type	type;

	type = (*token)->type;
	// Check for multiple redirections of same type
	if ((type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC) && cmd->in_type != 0)
		return (syntax_error("syntax error: multiple input redirections", NULL), 0);
	if ((type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND) && cmd->out_type != 0)
		return (syntax_error("syntax error: multiple output redirections", NULL), 0);
	// Check for missing filename after redirection
	if (!(*token)->next || (*token)->next->type != TOKEN_WORD)
		return (syntax_error("syntax error near unexpected token `newline'", NULL), 0);
	// Move to the filename token
	*token = (*token)->next;
	// Handle input redirection
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup((*token)->value);
		if (!cmd->infile)
			return (exit_error(ERR_MEM, NULL, 1, NULL), 0);
		cmd->in_type = (type == TOKEN_REDIRECT_IN) ? 1 : 2;
	}
	// Handle output redirection
	else
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = ft_strdup((*token)->value);
		if (!cmd->outfile)
			return (exit_error(ERR_MEM, NULL, 1, NULL), 0);
		cmd->out_type = (type == TOKEN_REDIRECT_OUT) ? 1 : 2;
	}
	return (1);
}*/