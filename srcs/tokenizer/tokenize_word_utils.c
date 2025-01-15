/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:09:45 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/13 13:34:21 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quoted_dollar_expansion(t_quote_state *state)
{
	char	*expanded;
	int		expanded_len;

	expanded = handle_dollar(state->input, &state->pos, state->shell);
	if (!expanded)
		return (0);
	expanded_len = ft_strlen(expanded);
	ft_strlcpy(state->result + state->len, expanded, expanded_len + 1);
	state->len += expanded_len;
	free(expanded);
	return (1);
}

static int	handle_quoted_dollar(t_quote_state *state)
{
	char	*temp;

	temp = ft_substr(state->input + state->pos, 0, 1);
	if (!temp)
		return (0);
	if (ft_isalnum(state->input[state->pos + 1]) || state->input[state->pos
			+ 1] == '_' || state->input[state->pos + 1] == '?')
	{
		free(temp);
		return (handle_quoted_dollar_expansion(state));
	}
	state->result[state->len++] = state->input[state->pos++];
	free(temp);
	return (1);
}

int	process_char_within_quotes(t_quote_state *state, char quote_type)
{
	if (quote_type == '"' && state->input[state->pos] == '\\')
	{
		if (!handle_escape_sequence(state, quote_type))
			return (0);
	}
	else if (quote_type == '"' && state->input[state->pos] == '$')
	{
		if (!handle_quoted_dollar(state))
			return (0);
	}
	else
	{
		state->result[state->len++] = state->input[state->pos];
		state->pos++;
	}
	return (1);
}
