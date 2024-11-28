/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:14 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 13:08:18 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function retrieves the value of an environment variable. \ 
It returns the value as a string if found, or NULL if not found.*/
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

/*This function finds the index of an environment variable. \ 
It returns the index of the variable if found, or -1 if not found.*/
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

char	**copy_env(char **envp)
{
	char	**new_env;
	int		i;
	int		size;

	size = 0;
	while (envp && envp[size])
		size++;
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

int	count_env_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

/*This function updates the SHLVL of the Minishell program*/
void	update_shell_level(t_shell *shell)
{
	char	*current_level;
	int		level;
	char	*new_level;

	current_level = get_env_value("SHLVL", shell);
	if (!current_level || !*current_level)
		level = 1;
	else
	{
		level = ft_atoi(current_level);
		if (level < 0)
			level = 0;
		else if (level >= 999)
			level = 1;
		level++;
	}
	new_level = ft_itoa(level);
	if (!new_level)
		return ;
	update_env_value("SHLVL", new_level, shell);
	free(new_level);
}
