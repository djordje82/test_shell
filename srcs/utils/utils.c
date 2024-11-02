#include "minishell.h"

int	is_whitespace(char c)
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
	return (ft_strchr(SPECIAL, c) != NULL);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_valid_identifier(char *name)
{
    int	i;

    if (!name || !*name || ft_isdigit(*name))
        return (0);
    i = 0;
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }
    return (1);
}