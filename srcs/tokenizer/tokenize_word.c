/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/20 16:19:17 by jadyar           ###   ########.fr       */
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

static int	process_quote_content(t_quote_state *state, char quote_type)
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

static int	process_quotes(const char *input, int *pos, char *result, int *len)
{
	t_quote_state	state;
	char			quote_type;

	state.input = input;
	state.result = result;
	state.pos = *pos;
	state.len = *len;
	quote_type = state.input[state.pos++];
	if (!process_quote_content(&state, quote_type))
	{
		handle_quote_error(state.result);
		return (0);
	}
	*pos = state.pos;
	*len = state.len;
	return (1);
}

static int	process_word_content(const char *input, int *pos, char *result)
{
	int	len;

	len = 0;
	while (input[*pos])
	{
		if (input[*pos] == '\'' || input[*pos] == '"')
		{
			if (!process_quotes(input, pos, result, &len))
			{
				ft_putendl_fd("minishell: syntax error: unexpected EOF", 2);
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
