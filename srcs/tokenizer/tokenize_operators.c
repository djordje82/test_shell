/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operators.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:42 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/11 19:40:58 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_operator_type(char c)
{
	if (c == '\'')
		return (TOKEN_SQUOTE);
	if (c == '\"')
		return (TOKEN_DQUOTE);
	if (c == '$')
		return (TOKEN_ENV);
	if (c == '>')
		return (TOKEN_REDIR_OUT);
	if (c == '<')
		return (TOKEN_REDIR_IN);
	if (c == '|')
		return (TOKEN_PIPE);
	else
		return (TOKEN_UNKNOWN);
}

t_token	*create_operator_token(const char *value, t_token_type type,
		int advance, int *pos)
{
	t_token	*token;

	token = create_token(value, type);
	if (token && pos)
		*pos += advance;
	return (token);
}

t_token	*tokenize_double_operator(const char *input, int *pos,
		const char *op_str, t_token_type type)
{
	if (!input || !pos || !op_str)
		return (NULL);
	return (create_operator_token(op_str, type, 2, pos));
}

t_token	*tokenize_single_operator(const char *input, int *i)
{
	char			value[2];
	t_token_type	type;

	value[0] = input[*i];
	if (!input || !i)
		return (NULL);
	type = get_operator_type(input[*i]);
	if (type == TOKEN_UNKNOWN)
	{
		ft_putendl_fd("minishell: syntax error", 2);
		return (NULL);
	}
	return (create_operator_token(value, type, 1, i));
}
