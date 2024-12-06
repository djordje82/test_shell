/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/06 14:08:56 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_escaped_chars(char *input, int *pos, char *content, int *i)
{
	if (input[*pos] == '\\' && (input[*pos + 1] == '"' || input[*pos
				+ 1] == '\\' || input[*pos + 1] == '$'))
	{
		(*pos)++;
		content[(*i)++] = input[*pos];
		return (content);
	}
	content[(*i)++] = input[*pos];
	return (content);
}

static char	*process_quoted_content(char *input, int *pos, char quote_type,
		int *i)
{
	char	*content;

	content = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!content)
		return (NULL);
	while (input[*pos])
	{
		if (input[*pos] == quote_type)
		{
			if (input[*pos + 1] == quote_type)
			{
				(*pos)++;
				continue ;
			}
			(*pos)++;
			break ;
		}
		if (quote_type == '"')
			content = handle_escaped_chars(input, pos, content, i);
		else
			content[(*i)++] = input[*pos];
		(*pos)++;
	}
	content[*i] = '\0';
	return (content);
}

char	*extract_quoted(char *input, int *pos, char quote_type)
{
	char	*content;
	int		i;

	i = 0;
	if (!input || !pos)
		return (NULL);
	(*pos)++;
	content = process_quoted_content(input, pos, quote_type, &i);
	return (content);
}

static char	*handle_quote_expansion(char *value, char quote_type,
		t_shell *shell)
{
	char	*expanded;

	if (quote_type == '\'')
		expanded = ft_strdup(value);
	else if (quote_type == '"' && *value)
	{
		expanded = expand_env_vars(value, shell);
		free(value);
	}
	else
		expanded = ft_strdup(value);
	return (expanded);
}

t_token	*tokenize_quoted_str(char *input, int *i, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;
	char	quote_type;

	quote_type = input[*i];
	value = extract_quoted(input, i, quote_type);
	if (!value)
		return (cleanup_and_exit(ERR_QUOTE, NULL, 1, shell), NULL);
	expanded = handle_quote_expansion(value, quote_type, shell);
	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
