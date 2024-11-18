/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:07:46 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function returns the type of a character based on its value. It checks for whitespace | pipe | redirection | quote | double quote | semicolon | normal character.*/
t_char_type	identify_shell_char(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return (CHAR_WHITESPACE);
	if (c == '|')
		return (CHAR_PIPE);
	if (c == '<')
		return (CHAR_REDIR_IN);
	if (c == '>')
		return (CHAR_REDIR_OUT);
	if (c == '\'')
		return (CHAR_QUOTE);
	if (c == '\"')
		return (CHAR_DQUOTE);
	if (c == ';')
		return (CHAR_SEMICOLON);
	return (CHAR_NORMAL);
}
/*TO TEST
t_char_type	identify_shell_char(char c)
{
	if ((c > 0 && c < 32) && c != ' ' && c != '\t' && c != '\n' && c != '\r')
		return (CHAR_INVALID);
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return (CHAR_WHITESPACE);
	if (c == '|')
		return (CHAR_PIPE);
	if (c == '<')
		return (CHAR_REDIR_IN);
	if (c == '>')
		return (CHAR_REDIR_OUT);
	if (c == '\'')
		return (CHAR_QUOTE);
	if (c == '\"')
		return (CHAR_DQUOTE);
	if (c == ';')
		return (CHAR_SEMICOLON);
	return (CHAR_NORMAL);
}*/