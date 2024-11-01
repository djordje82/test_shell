#include "minishell.h"

static t_token	*create_double_token(char *value, t_token_type type, int *i)
{
	t_token	*token;

	token = create_token(value, type);
	(*i)++;
	return (token);
}

t_token	*handle_operator(char *input, int *i)
{
	char	*value;
	t_token	*token;

	if (input[*i] == '>' && input[*i + 1] == '>')
		return (create_double_token(">>", TOKEN_APPEND, i));
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (create_double_token("<<", TOKEN_HEREDOC, i));
	value = ft_substr(input, *i, 1);
	if (!value)
		return (NULL);
	token = create_token(value, get_token_type(input[*i]));
	free(value);
	return (token);
}