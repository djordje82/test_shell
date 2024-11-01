#include "minishell.h"

int	is_whitespace(char c)
{
	return (ft_strchr(WHITESPACE, c) != NULL);
}

int	is_metacharacter(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || 
			c == '\t' || c == '\n' || c == '\'' || c == '\"');
}

