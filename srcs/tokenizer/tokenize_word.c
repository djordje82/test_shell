/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/12 18:59:23 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_char_within_quotes(t_quote_state *state, char quote_type)
{
	if (quote_type == '"' && state->input[state->pos] == '\\')
	{
		if (!handle_escape_sequence(state, quote_type))
			return (0);
	}
	else
	{
		state->result[state->len++] = state->input[state->pos];
		state->pos++;
	}
	return (1);
}

static int	process_quoted_section(t_quote_state *state, char quote_type)
{
	while (state->input[state->pos])
	{
		if (state->input[state->pos] == quote_type)
		{
			state->pos++;
			return (1);
		}
		if (!process_char_within_quotes(state, quote_type))
			return (0);
	}
	return (0);
}

static int	handle_quoted(const char *input, int *pos, char *result, int *len)
{
	t_quote_state	state;
	char			quote_type;

	state.input = input;
	state.result = result;
	state.pos = *pos;
	state.len = *len;
	quote_type = state.input[state.pos++];
	if (!process_quoted_section(&state, quote_type))
	{
		print_quote_error(state.result);
		return (0);
	}
	*pos = state.pos;
	*len = state.len;
	return (1);
}

static int	process_word_content(const char *input, int *pos, char *result,
		t_shell *shell)
{
	int		len;
	char	*expanded;

	len = 0;
	while (input[*pos])
	{
		if (input[*pos] == '\'' || input[*pos] == '"')
		{
			if (!handle_quoted(input, pos, result, &len))
			{
				ft_putendl_fd("minishell: syntax error: unexpected EOF", 2);
				return (0);
			}
		}
		else if (is_word_delimiter(input[*pos]))
			break ;
		else
		{
			expanded = ft_substr(input, *pos, 1);
			if (expanded[0] == '$')
			{
				free(expanded);
				expanded = handle_dollar(input, pos, shell);
				ft_strlcpy(result + len, expanded, ft_strlen(expanded) + 1);
				len += ft_strlen(expanded);
			}
			else
			{
				result[len++] = input[(*pos)++];
				free(expanded);
			}
		}
	}
	result[len] = '\0';
	return (1);
}

t_token	*tokenize_word(const char *input, int *pos, t_shell *shell)
{
	char	buffer[1024];
	t_token	*token;

	if (!process_word_content(input, pos, buffer, shell))
		return (NULL);
	token = create_token(buffer, TOKEN_WORD);
	return (token);
}
