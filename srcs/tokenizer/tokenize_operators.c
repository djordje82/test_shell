/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_operators.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:42 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:58:44 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_double_operator(const char *input, int *pos,
	const char *op_str, t_token_type type)
{
	t_token	*token;
	char	*value;

	(void)input;  // Silence unused parameter warning
	value = ft_strdup(op_str);
	if (!value)
		return (NULL);
	token = create_token(value, type);
	free(value);
	if (!token)
		return (NULL);
	*pos += 2;
	return (token);
}

static t_token	*handle_single_operator(const char *input, int *i)
{
	char		*value;
	t_token		*token;
	t_token_type	type;

	value = ft_substr(input, *i, 1);
	if (!value)
		return (NULL);
	type = get_token_type(input[*i]);
	token = create_token(value, type);
	free(value);
	if (!token)
		return (NULL);
	(*i)++;
	return (token);
}

t_token	*tokenize_operator(const char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (handle_double_operator(input, i, ">>", TOKEN_APPEND));
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (handle_double_operator(input, i, "<<", TOKEN_HEREDOC));
	return (handle_single_operator(input, i));
}
