/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:48:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:07:52 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	t_char_type	type;

	type = find_special_chars(c);
	return (type != CHAR_NORMAL);
}

int	is_whitespace(char c)
{
	return (find_special_chars(c) == CHAR_WHITESPACE);
}

int	is_quote(char c)
{
	t_char_type	type;

	type = find_special_chars(c);
	return (type == CHAR_SQUOTE || type == CHAR_DQUOTE);
}

int	is_word_delimiter(char c)
{
	return (is_whitespace(c) || 
		(is_special_char(c) && !is_quote(c)));
}
