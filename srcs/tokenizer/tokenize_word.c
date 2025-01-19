/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/16 16:47:23 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	handle_quoted(t_quote_state *state)
{
	state->quote_type = state->input[state->pos++];
	if (!process_quoted_section(state, state->quote_type))
	{
		print_quote_error(state->result);
		return (0);
	}
	return (1);
}

static int	process_regular_char(t_quote_state *state)
{
	char	*expanded;

	expanded = ft_substr(state->input, state->pos, 1);
	if (expanded[0] == '$')
	{
		free(expanded);
		expanded = handle_dollar(state->input, &state->pos, state->shell);
		ft_strlcpy(state->result + state->len, expanded, 
			ft_strlen(expanded) + 1);
		state->len += ft_strlen(expanded);
		free(expanded);
	}
	else
	{
		state->result[state->len++] = state->input[state->pos++];
		free(expanded);
	}
	return (1);
}

static int	process_word_content(const char *input, int *pos, char *result,
		t_shell *shell)
{
	t_quote_state	state;

	state.input = input;
	state.result = result;
	state.pos = *pos;
	state.len = 0;
	state.shell = shell;
	while (state.input[state.pos])
	{
		if (state.input[state.pos] == '\'' || state.input[state.pos] == '"')
		{
			if (!handle_quoted(&state))
			{
				ft_putendl_fd("minishell: syntax error: unexpected EOF", 2);
				return (0);
			}
		}
		else if (is_word_delimiter(state.input[state.pos]))
			break ;
		else if (!process_regular_char(&state))
			return (0);
	}
	state.result[state.len] = '\0';
	*pos = state.pos;
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
