/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:46:48 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to append a part of the input string to the result.*/
static char	*append_word_part(char *result, const char *input, 
	int start, int len)
{
	char	*temp;
	char	*new_result;

	temp = ft_substr(input, start, len);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	new_result = ft_strjoin_free(result, temp);
	free(temp);
	return (new_result);
}

/*This function is used to process a quoted segment of the input string. It appends the part of the string that is not quoted to the result.*/
static char	*process_quoted_segment(char *result, const char *input,
	int *start, int *len)
{
	char	quote_type;
	int		quote_pos;
	char	*temp;

	if (*len > 0)
	{
		result = append_word_part(result, input, *start, *len);
		if (!result)
			return (NULL);
		*start += *len;
		*len = 0;
	}
	quote_type = input[*start + *len];
	quote_pos = *start + *len;
	temp = extract_quoted((char *)input, &quote_pos, quote_type);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	result = ft_strjoin_free(result, temp);
	free(temp);
	*len = quote_pos - *start - *len;
	*start += *len;
	return (result);
}

/*This function is used to extract a word from the input string. It handles both quoted and unquoted parts of the string.*/
char	*extract_word(const char *input, int *pos)
{
	char	*result;
	int		start;
	int		len;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	start = *pos;
	len = 0;
	while (input[start + len])
	{
		if (input[start + len] == '\'' || input[start + len] == '"')
		{
			result = process_quoted_segment(result, input, &start, &len);
			if (!result)
				return (NULL);
			continue ;
		}
		if (is_whitespace(input[start + len]) || 
			(is_special_char(input[start + len]) && 
			!is_quote(input[start + len])))
			break ;
		len++;
	}
	if (len > 0)
		result = append_word_part(result, input, start, len);
	*pos = start + len;
	return (result && *result ? result : NULL);
}

/*This function is used to extract a quoted string from the input string. It handles both single and double quotes.*/
char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;
	char	*result;

	(*i)++;
	start = *i;
	len = 0;

	while (input[*i] && input[*i] != quote_type)
	{
		len++;
		(*i)++;
	}
	if (!input[*i])
	{
		return (NULL);
	}
	result = ft_substr(input, start, len);
	(*i)++;

	return (result);
}

static t_token	*create_wildcard_token_list(char **matches)
{
	t_token	*first;
	t_token	*current;
	int		i;

	if (!matches[0])
		return (NULL);
	first = create_token(matches[0], TOKEN_WORD);
	if (!first)
		return (NULL);
	current = first;
	i = 1;
	while (matches[i])
	{
		current->next = create_token(matches[i], TOKEN_WORD);
		if (!current->next)
		{
			cleanup_token_list(first);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	return (first);
}

t_token	*tokenize_word(const char *input, int *i, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;
	char	**matches;

	value = extract_word(input, i);
	if (!value)
		return (NULL);

	expanded = expand_env_vars(value, shell);
	free(value);
	if (!expanded)
		return (NULL);

	matches = expand_wildcards(expanded);
	if (matches)
	{
		token = create_wildcard_token_list(matches);
		ft_free_array((void **)matches, -1);
		free(expanded);
		return (token);
	}

	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
