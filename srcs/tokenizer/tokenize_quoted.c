/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/12 13:19:43 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_empty_quotes(const char *input, int *pos)
{
	if (input[*pos + 1] == input[*pos])
	{
		*pos += 2;
		return (1);
	}
	return (0);
}

static char	*process_quote(const char *input, int *pos, t_shell *shell,
		char quote_type)
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

static int	append_to_buffer(char *buffer, const char *processed, int len)
{
	if (len + ft_strlen(processed) >= 1024)
		return (0);
	ft_strlcat(buffer + len, processed, 1024 - len);
	return (len + ft_strlen(processed));
}

t_token	*tokenize_quotes(const char *input, int *pos, t_shell *shell)
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
