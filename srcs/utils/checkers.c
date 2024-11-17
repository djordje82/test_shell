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

	type = identify_shell_char(c);
	return (type != CHAR_NORMAL);
}

int	is_whitespace(char c)
{
	return (identify_shell_char(c) == CHAR_WHITESPACE);
}

int	is_quote(char c)
{
	t_char_type	type;

	type = identify_shell_char(c);
	return (type == CHAR_QUOTE || type == CHAR_DQUOTE);
}

int	has_equals_sign(char *str)
{
	while (*str)
	{
		if (*str == '=')
			return (1);
		str++;
	}
	return (0);
}