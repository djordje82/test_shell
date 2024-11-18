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

/*This function creates a token for a double-character operators (>> or <<)*/
static t_token	*tokenize_double_operator(const char *input,
		int *pos,
		const char *op_str,
		t_token_type type)
{
	t_token	*token;
	char	*value;

	(void)input;
	if (!pos || !op_str)
		return (NULL);
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

/*This function creates a token for a single-character operator (>, <, |, etc.)*/
static t_token	*tokenize_single_operator(const char *input, int *i)
{
	char		*value;
	t_token		*token;
	t_token_type	type;

	if (!input || !i)
		return (NULL);
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

/*This function determines the type of operator based on the current position in the input string and creates the appropriate token.*/
t_token	*tokenize_operator(const char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (tokenize_double_operator(input, i, ">>", TOKEN_APPEND));
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (tokenize_double_operator(input, i, "<<", TOKEN_HEREDOC));
	return (tokenize_single_operator(input, i));
}

