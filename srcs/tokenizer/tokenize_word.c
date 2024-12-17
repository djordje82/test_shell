/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 11:59:04 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_quotes(const char *input, int *pos, char *result, int *len)
{
	char	quote_type;

	quote_type = input[*pos];
	(*pos)++;
	while (input[*pos])
	{
		if (input[*pos] == quote_type)
		{
			(*pos)++;
			return (1);
		}
		if (quote_type == '"' && input[*pos] == '\\' \
			&& (input[*pos + 1] == '"' || input[*pos + 1] == '$' \
				|| input[*pos + 1] == '\\'))
		{
			(*pos)++;
			result[(*len)++] = input[(*pos)++];
		}
		else
		{
			result[(*len)++] = input[(*pos)];
			(*pos)++;
		}
	}
	handle_quote_error(result);
	return (1);
}

/* char	*append_word_part(char *result, const char *input, int start, int len)
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

char	*append_unquoted_part(char *result, const char *input, int *start,
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

char	*handle_quote_error(char *result)
{
	ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
	g_exit_status = 2;
	free(result);
	return (NULL);
} */

static int	process_word_content(const char *input, int *pos, char *result)
{
	int		len;

	len = 0;
	while (input[*pos])
	{
		if (input[*pos] == '\'' || input[*pos] == '"')
		{
			if (!process_quotes(input, pos, result, &len))
			{
				ft_putendl_fd("minishell: syntax error: \
					unclosed quotes", STDERR_FILENO);
				return (0);
			}
		}
		else if (is_word_delimiter(input[*pos]))
			break ;
		else
			result[len++] = input[(*pos)++];
	}
	result[len] = '\0';
	return (1);
}

t_token	*tokenize_word(const char *input, int *pos, t_shell *shell)
{
	char	buffer[1024];
	char	*expanded;
	t_token	*token;

	if (!process_word_content(input, pos, buffer))
		return (NULL);
	if (buffer[0] == '\0')
		expanded = ft_strdup("");
	else
		expanded = expand_env_vars(buffer, shell);
	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
