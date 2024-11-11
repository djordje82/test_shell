#include "minishell.h"

static int	validate_pipe_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE)
		return (0);
	while (tokens && tokens->next)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next->type == TOKEN_PIPE)
			return (0);
		tokens = tokens->next;
	}
	if (tokens && tokens->type == TOKEN_PIPE)
		return (0);
	return (1);
}

t_command	*parse_command(t_token **token)
{
	t_command	*cmd;
	t_token		*temp;

	//printf("Debug: Starting parse_command\n");
	temp = *token;
	while (temp)
	{
		//printf("Debug: Token type %d, value '%s'\n", temp->type, temp->value);
		temp = temp->next;
	}
	cmd = create_cmd_node();
	if (!cmd)
		return (NULL);
	
	while (*token && (*token)->type != TOKEN_PIPE)
	{
		//printf("Debug: Processing token type %d, value '%s'\n", (*token)->type, (*token)->value);
		
		if ((*token)->type == TOKEN_REDIRECT_IN || 
			(*token)->type == TOKEN_REDIRECT_OUT ||
			(*token)->type == TOKEN_APPEND || 
			(*token)->type == TOKEN_HEREDOC)
		{
			if (!parse_redirections(token, cmd))
				return (free_cmd_list(cmd), NULL);
		}
		else if ((*token)->type == TOKEN_WORD || 
				 (*token)->type == TOKEN_QUOTE || 
				 (*token)->type == TOKEN_DQUOTE)
		{
			//printf("Debug: Parsing argument token: '%s' of type %d\n", (*token)->value, (*token)->type);
			if (!parse_arguments(token, cmd))
				return (free_cmd_list(cmd), NULL);
			//printf("Debug: After parsing, args[0]: '%s'\n", cmd->args[0]);
			continue;

		}
		if (*token && (*token)->type != TOKEN_PIPE)
			*token = (*token)->next;
	}
	//printf("Debug: Command parsed successfully\n");
	//printf("Debug: Command args:\n");
	int i = 0;
	while (cmd->args && cmd->args[i])
	{
		//printf("Debug: args[%d] = '%s'\n", i, cmd->args[i]);
		i++;
	}
	return (cmd);
}

static int	build_command_list(t_token *tokens, t_shell *shell)
{
	t_command	*cmd;
	t_token		*current;

	current = tokens;
	shell->cmnd_lst = NULL;
	while (current)
	{
		cmd = parse_command(&current);
		if (!cmd)
		{
            if (shell->cmnd_lst)
                free_cmd_list(shell->cmnd_lst);
            shell->cmnd_lst = NULL;
            return (0);
        }
		add_cmd_node(&shell->cmnd_lst, cmd);
		if (current && current->type == TOKEN_PIPE)
			current = current->next;
	}
	return (1);
}

int parse_tokens(t_shell *shell)
{
    if (!shell->tokens)
        return (1);
    
    if (!validate_pipe_syntax(shell->tokens))
    {
        exit_error(ERR_SYNTAX_PIPE, NULL, 2, shell);
        g_exit_status = 2;  // Set exit status for syntax error
        return (0);         // Return but don't exit
    }
    
    if (!build_command_list(shell->tokens, shell))
    {
        g_exit_status = 2;  // Set exit status for syntax error
        return (0);         // Return but don't exit
    }
    
    return (1);
}
/*int	parse_tokens(t_shell *shell)
{
	if (!shell->tokens)
		return (1);
	if (!validate_pipe_syntax(shell->tokens))
		return (exit_error(ERR_SYNTAX_PIPE, NULL, 2, shell), 0);
	if (!build_command_list(shell->tokens, shell))
		return (0);
	return (1);
}*/