/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/29 17:28:09 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function returns the type of a character based on its value. \ 
It checks for whitespace | pipe | redirection | quote \ 
| double quote | semicolon | normal character.*/
t_char_type	find_special_chars(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' \
		|| c == '\v' || c == '\f')
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

