/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:08:10 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function creates a new environment variable string by concatenating the name and value with an equal sign. It returns the new string or NULL if there is an error.*/
char	*create_env_string(char *name, char *value)
{
	char	*result;
	size_t	name_len;
	size_t	value_len;

	if (!name || !value)
		return (NULL);
	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	result = malloc(name_len + value_len + 2);
	if (!result)
		return (NULL);
	ft_strlcpy(result, name, name_len + 1);
	result[name_len] = '=';
	ft_strlcpy(result + name_len + 1, value, value_len + 1);
	return (result);
}

/*This function updates the value of an environment variable in the shell's environment array. It returns 0 on success, or 1 if the variable is not found or if there is an error.*/
int	update_env_value(char *name, char *value, t_shell *shell)
{
	int		index;
	char	*new_str;

	if (!name || !value || !shell->envp)
		return (1);
	index = find_env_index(name, shell->envp);
	if (index < 0)
		return (1);
	new_str = create_env_string(name, value);
	if (!new_str)
		return (1);
	free(shell->envp[index]);
	shell->envp[index] = new_str;
	return (0);
}

