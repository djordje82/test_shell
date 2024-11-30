/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:12:38 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/30 17:15:53 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function extracts the name of an environment variable from a string.*/
char	*extract_env_var_name(const char *str)
{
	int	len;

	if (!str || !*str)
		return (NULL);
	if (str[0] == '?')
		return (ft_strdup("?"));
	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	if (len == 0)
		return (NULL);
	return (ft_substr(str, 0, len));
}

/*This function extracts the value of an assignment from a given string. \
It returns a new string containing the value, \
or an empty string if no assignment is found.*/
char	*extract_env_var_value(char *arg)
{
	char	*equals;

	if (!arg)
		return (NULL);
	equals = ft_strchr(arg, '=');
	if (!equals)
		return (ft_strdup(""));
	return (ft_strdup(equals + 1));
}

/*This function checks if a variable name is valid. 
It checks if the name is not empty | if the first character
 is a digit | 
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
