#include "minishell.h"

t_char_type get_char_type(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        return (CHAR_WHITESPACE);
    if (c == '|')
        return (CHAR_PIPE);
    if (c == '<')
        return (CHAR_REDIR_IN);
    if (c == '>')
        return (CHAR_REDIR_OUT);
    if (c == '\'')
        return (CHAR_QUOTE);
    if (c == '\"')
        return (CHAR_DQUOTE);
    if (c == ';')
        return (CHAR_SEMICOLON);
    if (c == '\\')
        return (CHAR_ESCAPE);
    return (CHAR_NORMAL);
}

// Replace multiple is_* functions with one function
int is_special_char(char c)
{
    t_char_type type = get_char_type(c);
    return (type != CHAR_NORMAL);
}

// Keep only these essential helpers
int is_whitespace(char c)
{
    return (get_char_type(c) == CHAR_WHITESPACE);
}

int is_quote(char c)
{
    t_char_type type = get_char_type(c);
    return (type == CHAR_QUOTE || type == CHAR_DQUOTE);
}

int	is_valid_identifier(char *name)
{
	int	i;
	int	has_equals;

	if (!name || !*name || ft_isdigit(name[0]))
		return (0);

	has_equals = 0;
	i = 0;
	while (name[i])
	{
		if (name[i] == '=')
		{
			has_equals = 1;
			break;
		}
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}

	return (1);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args &&args[i])
		i++;
	return (i);
}

/*int	is_whitespace(char c)
{
	return (ft_strchr(TOKEN_DELIMITERS, c) != NULL);
}

int	is_metacharacter(char c)
{
	return (ft_strchr(SPECIAL, c) != NULL || is_whitespace(c) || 
			c == '\'' || c == '\"');
}

int	is_operator(char c)
{
	return (ft_strchr(OPERATORS, c) != NULL);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}*/