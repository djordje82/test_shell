/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:55:26 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//TO DO: SPLIT
int	add_new_var(char *arg, t_shell *shell)
{
	char	**new_env;
	int		size;
	int		i;

	size = count_env_vars(shell->envp);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (0);
	i = 0;
	while (shell->envp[i])
	{
		new_env[i] = ft_strdup(shell->envp[i]);
		if (!new_env[i])
		{
			ft_free_array((void **)new_env, i);
			return (0);
		}
		i++;
	}
	new_env[i] = ft_strdup(arg);
	if (!new_env[i])
	{
		ft_free_array((void **)new_env, i);
		return (0);
	}
	new_env[i + 1] = NULL;
	ft_free_array((void **)shell->envp, -1);
	shell->envp = new_env;
	return (1);
}

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

int	remove_env_var(char *name, t_shell *shell)
{
	int	i;
	int	j;
	int	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, name_len) == 0
			&& (shell->envp[i][name_len] == '='
			|| shell->envp[i][name_len] == '\0'))
		{
			free(shell->envp[i]);
			j = i;
			while (shell->envp[j + 1])
			{
				shell->envp[j] = shell->envp[j + 1];
				j++;
			}
			shell->envp[j] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
