#include "minishell.h"

/*static t_token	*create_double_token(char *value, t_token_type type, int *i)
{
	t_token	*token;

	token = create_token(value, type);
	(*i)++;
	return (token);
}*/

t_token *handle_operator(const char *input, int *i)
{
    char        *value;
    t_token         *token;
    t_token_type    type;

    // Handle double operators
    if (input[*i] == '>' && input[*i + 1] == '>')
    {
        token = create_token(">>", TOKEN_APPEND);
        if (!token)
            return NULL;
        (*i) += 2; // Skip both characters
        return token;
    }
    if (input[*i] == '<' && input[*i + 1] == '<')
    {
        token = create_token("<<", TOKEN_HEREDOC);
        if (!token)
            return NULL;
        (*i) += 2; // Skip both characters
        return token;
    }
    // Handle single operators
    value = ft_substr(input, *i, 1);
    if (!value)
        return NULL;
        
    type = get_token_type(input[*i]);
    token = create_token(value, type);
    free(value);
    if (!token)
        return NULL;
    (*i)++; // Add this line to increment position after single operator
    return token;
}