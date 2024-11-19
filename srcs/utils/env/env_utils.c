/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:14 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:08:08 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function retrieves the value of an environment variable from the shell's environment array. It returns the value as a string if found, or NULL if not found.*/
char	*get_env_value(char *name, t_shell *shell)
{
	int		i;
	int		len;
	char	*env_str;

	if (!name || !shell->envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=')
		{
			env_str = shell->envp[i] + len + 1;
			return (env_str);
		}
		i++;
	}
	return (NULL);
}

/*This function finds the index of an environment variable in an array of environment variables. It returns the index of the variable if found, or -1 if not found.*/
int	find_env_index(char *name, char **envp)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/*This function extracts the name of an environment variable from a string. It returns the name as a new string if found, or NULL if not found.*/
char	*extract_env_var_name(const char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}