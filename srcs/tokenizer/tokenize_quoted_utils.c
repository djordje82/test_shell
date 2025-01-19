/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 13:19:20 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/17 14:45:22 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (quote_type == '"' && input[*start] == '\\' && (input[*start
					+ 1] == '"' || input[*start + 1] == '$'))
			(*start)++;
		content[i++] = input[(*start)++];
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

int	append_to_buffer(char *buffer, const char *processed, int len)
{
	if (len + ft_strlen(processed) >= 1024)
		return (0);
	ft_strlcat(buffer + len, processed, 1024 - len);
	return (len + ft_strlen(processed));
}

int	append_unquoted_chars(const char *input, int *pos, char *buffer, 
				int start_len)
{
	int	len;

	len = start_len;
	while (input[*pos] && !is_word_delimiter(input[*pos]))
	{
		if (len + 1 >= 1024)
			return (-1);
		buffer[len++] = input[*pos];
		(*pos)++;
	}
	buffer[len] = '\0';
	return (len);
}
