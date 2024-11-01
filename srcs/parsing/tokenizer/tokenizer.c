#include "minishell.h"

static t_token	*handle_word(char *input, int *i)
{
	char	*value;
	t_token	*token;

	value = extract_word(input, i);
	if (!value)
		return (NULL);
	token = create_token(value, TOKEN_WORD);
	free(value);
	return (token);
}

static t_token	*handle_quote(char *input, int *i, t_shell *shell)
{
	char	*value;
	t_token	*token;
	char	quote;

	quote = input[*i];
	value = extract_quoted(input, i, quote);
	if (!value)
		return (exit_error(ERR_QUOTE, NULL, 1, shell), NULL);
	token = create_token(value, TOKEN_WORD);
	free(value);
	return (token);
}

static t_token	*get_next_token(char *input, int *i, t_shell *shell)
{
	if (is_whitespace(input[*i]))
		return ((*i)++, NULL);
	if (input[*i] == '\'' || input[*i] == '\"')
		return (handle_quote(input, i, shell));
	if (is_metacharacter(input[*i]))
		return (handle_operator(input, i));
	return (handle_word(input, i));
}

t_token	*tokenize_input(char *input, t_shell *shell)
{
	int		i;
	t_token	*tokens;
	t_token	*new_token;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		new_token = get_next_token(input, &i, shell);
		if (new_token)
			add_token(&tokens, new_token);
		if (input[i])
			i++;
	}
	return (tokens);
}