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

/*This function returns the type of a character based on its value. \ 
It checks for whitespace | pipe | redirection | quote \ 
| double quote | semicolon | normal character.*/
t_char_type	find_special_chars(char c)
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
		return (CHAR_SQUOTE);
	if (c == '\"')
		return (CHAR_DQUOTE);
	return (CHAR_NORMAL);
}

/*This function checks if a variable name is valid. \ 
It checks if the name is not empty | if the first character is a digit | \ 
if the name contains an equal sign.*/
int	validate_env_var(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(name[0]))
		return (0);
	i = 0;
	while (name[i])
	{
		if (name[i] == '=')
		{
			break ;
		}
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}