/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:20:46 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:41:02 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_word_part(char *result, const char *input, int start, int len)
{
	char	*temp;
	char	*new_result;

	if (!result || !input)
		return (NULL);
	temp = ft_substr(input, start, len);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	new_result = ft_strjoin_free(result, temp);
	if (!new_result)
		return (NULL);
	free(temp);
	return (new_result);
}

static char	*append_unquoted_part(char *result, const char *input, int *start,
		int *len)
{
	if (!result || !input || !start || !len)
		return (NULL);
	if (*len > 0)
	{
		result = append_word_part(result, input, *start, *len);
		if (!result)
			return (NULL);
		*start += *len;
		*len = 0;
	}
	return (result);
}

//TO DO: SPLIT
char	*process_quoted_segment(char *result, const char *input, int *start,
		int *len)
{
	char	quote_type;
	int		quote_pos;
	char	*quoted_content;

	if (!result || !input || !start || !len)
		return (NULL);
	result = append_unquoted_part(result, input, start, len);
	if (!result)
		return (NULL);
	quote_type = input[*start + *len];
	quote_pos = *start + *len;
	quoted_content = extract_quoted((char *)input, &quote_pos, quote_type);
	if (!quoted_content)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		g_exit_status = 2;
		free(result);
		return (NULL);
	}
	result = ft_strjoin_free(result, quoted_content);
	free(quoted_content);
	if (!result)
		return (NULL);
	*len = quote_pos - *start;
	*start = quote_pos;
	return (result);
}

//TO DO: SPLIT
static char	*process_word_content(const char *input, int *start, int *len)
{
	char	*result;
	char	curr_char;

	if (!input || !start || !len)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
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
	if (*len > 0)
	{
		result = append_word_part(result, input, *start, *len);
		if (!result)
			return (NULL);
	}
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
