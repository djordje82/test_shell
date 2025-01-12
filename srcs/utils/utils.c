/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:04 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/08 22:57:23 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	t_char_type	type;

	type = find_special_chars(c);
	return (type != CHAR_NORMAL);
}

int	is_quote(char c)
{
	t_char_type	type;

	type = find_special_chars(c);
	return (type == CHAR_SQUOTE || type == CHAR_DQUOTE);
}

int	is_word_delimiter(char c)
{
	return (ft_is_whitespace(c) || 
		(is_special_char(c) && !is_quote(c)));
}

t_char_type	find_special_chars(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f')
		return (CHAR_WHITESPACE);
	if (c == '|')
		return (CHAR_PIPE);
	if (c == '<')
		return (CHAR_REDIR_IN);
	if (c == '>')
		return (CHAR_REDIR_OUT);
	if (c == '\'')
		return (CHAR_SQUOTE);
	if (c == '\"')
		return (CHAR_DQUOTE);
	return (CHAR_NORMAL);
}
