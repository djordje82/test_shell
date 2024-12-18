/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 12:43:51 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_quotes(const char *input, int *pos, char *result, int *len)
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
	//handle_quote_error(result);
	return (1);
}

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
t_token	*tokenize_word(const char *input, int *pos, t_shell *shell)
{
	char	buffer[1024];
	char	buffer[1024];
	char	*expanded;
	t_token	*token;

	if (!process_word_content(input, pos, buffer))
	if (!process_word_content(input, pos, buffer))
		return (NULL);
	if (buffer[0] == '\0')
		expanded = ft_strdup("");
	else
		expanded = expand_env_vars(buffer, shell);
	if (buffer[0] == '\0')
		expanded = ft_strdup("");
	else
		expanded = expand_env_vars(buffer, shell);
	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
