/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/19 15:04:51 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static char	*handle_escaped_chars(char *input, int *pos, char *content, int *i)
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
} */

char	*process_quoted_content(const char *input, int *start, int *len)
{
	char	quote_type;
	char	*content;
	int		i;

	quote_type = input[*start];
	content = malloc(ft_strlen(input) + 1);
	i = 0;
	if (!content)
		return (NULL);
	(*start)++;
	while (input[*start])
	{
		if (input[*start] == quote_type)
		{
			(*start)++;
			break ;
		}
		/* if (quote_type == '"' && input[*start] == '\\' && (input[*start
					+ 1] == '"' || input[*start + 1] == '$'))
			(*start)++;
		content[i++] = input[(*start)++]; */
		if (quote_type == '"' && input[*start] == '\\' && (input[*start + 1] == '"' || input[*start + 1] == '$'))
        {
            (*start)++;
            content[i++] = input[*start];
        }
        else
        {
            content[i++] = input[*start];
        }
        (*start)++;
	}
	content[i] = '\0';
	*len = i;
	return (content);
}

char	*extract_quoted(const char *input, int *pos, char quote_type)
{
	char	*content;
	int		len;

	len = 0;
	if (!input || !pos || input[*pos] != quote_type)
		return (NULL);
	content = process_quoted_content(input, pos, &len);
	if (!content)
		return (NULL);
	return (content);
}

/* static char	*handle_quote_expansion(char *value, char quote_type,
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
} */

t_token *tokenize_adjacent_quotes(const char *input, int *pos, t_shell *shell)
{
	char	buffer[1024];
	int		len;
	t_token	*token;
	char	*quoted;

	(void)shell;
	len = 0;
	while (input[*pos] == '\'' || input[*pos] == '"')
	{
		quoted = extract_quoted(input, pos, input[*pos]);
		if (!quoted)
			return (NULL);
		ft_strlcpy(buffer + len, quoted, sizeof(buffer) - len);
		len += ft_strlen(quoted);
		free(quoted);
	}
	token = create_token(buffer, TOKEN_WORD);
	return (token);
}

t_token	*tokenize_quoted_str(char *input, int *pos, t_shell *shell)
{
	char	*value;
	char	*processed;
	t_token	*token;
	char	quote_type;

	if (!input || !pos)
		return (NULL);
	quote_type = input[*pos];
	value = extract_quoted(input, pos, input[*pos]);
	if (!value)
		return (NULL);
	if (quote_type == '"')
		processed = expand_env_vars(value, shell);
	else
		processed = value;
	token = create_token(processed, TOKEN_WORD);
	if (quote_type == '"' && quote_type + 1 == '"')
		free(processed);
	return (token);
}
