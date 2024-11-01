#include "minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token_type	get_token_type(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<')
		return (TOKEN_REDIRECT_IN);
	if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	if (c == '\'')
		return (TOKEN_QUOTE);
	if (c == '\"')
		return (TOKEN_DQUOTE);
	if (c == '$')
		return (TOKEN_ENV);
	return (TOKEN_WORD);
}

char	*extract_word(char *input, int *i)
{
	int		start;
	int		len;

	start = *i;
	len = 0;
	while (input[*i] && !is_metacharacter(input[*i]))
	{
		len++;
		(*i)++;
	}
	(*i)--;
	return (ft_substr(input, start, len));
}

char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;

	(*i)++;
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote_type)
	{
		len++;
		(*i)++;
	}
	if (!input[*i])
		return (NULL);
	return (ft_substr(input, start, len));
}