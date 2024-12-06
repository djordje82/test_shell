/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:46 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/06 15:51:18 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_quoted_content(char *result, const char *input, int *start,
		int *len)
{
	char	quote_type;
	int		quote_pos;
	char	*quoted_content;

	quote_type = input[*start + *len];
	quote_pos = *start + *len;
	quoted_content = extract_quoted((char *)input, &quote_pos, quote_type);
	if (!quoted_content)
		return (handle_quote_error(result));
	result = ft_strjoin_free(result, quoted_content);
	free(quoted_content);
	if (!result)
		return (NULL);
	*len = quote_pos - *start;
	*start = quote_pos;
	return (result);
}

char	*process_quoted_segment(char *result, const char *input, int *start,
		int *len)
{
	if (!result || !input || !start || !len)
		return (NULL);
	result = append_unquoted_part(result, input, start, len);
	if (!result)
		return (NULL);
	return (process_quoted_content(result, input, start, len));
}

char	*process_word_loop(const char *input, char *result, int *start,
		int *len)
{
	char	curr_char;

	while (input[*start + *len])
	{
		curr_char = input[*start + *len];
		if (curr_char == '\'' || curr_char == '"')
		{
			result = process_quoted_segment(result, input, start, len);
			if (!result)
				return (NULL);
			continue ;
		}
		if (is_word_delimiter(curr_char))
			break ;
		(*len)++;
	}
	return (result);
}

static char	*process_word_content(const char *input, int *start, int *len)
{
	char	*result;

	if (!input || !start || !len)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	result = process_word_loop(input, result, start, len);
	if (!result)
		return (NULL);
	if (*len > 0)
		result = append_word_part(result, input, *start, *len);
	return (result);
}

char	*extract_word(const char *input, int *pos)
{
	char	*result;
	int		start;
	int		len;

	start = *pos;
	len = 0;
	result = process_word_content(input, &start, &len);
	if (!result || !*result)
	{
		free(result);
		return (NULL);
	}
	*pos = start + len;
	return (result);
}
