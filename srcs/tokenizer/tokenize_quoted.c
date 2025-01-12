/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/12 12:32:42 by dodordev         ###   ########.fr       */
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

static char	*process_quote(const char *input, int *pos, 
							t_shell *shell, char quote_type)
{
	char	*temp;
	char	*processed;

	temp = extract_quoted(input, pos, quote_type);
	if (!temp)
		return (NULL);
	if (quote_type == '"')
	{
		processed = expand_env_vars(temp, shell);
		free(temp);
		return (processed);
	}
	return (temp);
}

static int	handle_empty_quotes(const char *input, int *pos)
{
	if (input[*pos + 1] == input[*pos])
	{
		*pos += 2;
		return (1);
	}
	return (0);
}

static int	append_to_buffer(char *buffer, const char *processed, int len)
{
	if (len + ft_strlen(processed) >= 1024)
		return (0);
	ft_strlcat(buffer + len, processed, 1024 - len);
	return (len + ft_strlen(processed));
}

t_token	*tokenize_adjacent_quotes(const char *input, int *pos, t_shell *shell)
{
	char	buffer[1024];
	int		len;
	char	*processed;
	int		is_empty_quote;

	ft_bzero(buffer, sizeof(buffer));
	len = 0;
	is_empty_quote = 1;
	while (input[*pos] && (input[*pos] == '\'' || input[*pos] == '"'))
	{
		if (handle_empty_quotes(input, pos))
			continue ;
		is_empty_quote = 0;
		processed = process_quote(input, pos, shell, input[*pos]);
		if (!processed)
			return (NULL);
		len = append_to_buffer(buffer, processed, len);
		if (!len)
		{
			free(processed);
			return (NULL);
		}
		free(processed);
	}
	return (create_token(buffer, TOKEN_WORD));
}
