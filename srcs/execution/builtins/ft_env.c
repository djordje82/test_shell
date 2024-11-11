/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:54:00 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/17 11:54:20 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*create_env_string(char *name, char *value)
{
	char	*temp;
	char	*result;

	//printf("Debug: Creating env string for %s=%s\n", name, value);
	
	// Create "name=" string
	temp = ft_strjoin(name, "=");
	if (!temp)
	{
		//printf("Debug: Failed to create temp string\n");
		return (NULL);
	}
	
	// Add the value
	result = ft_strjoin(temp, value);
	//printf("Debug: Created env string: %s\n", result ? result : "NULL");
	
	// Free temporary string
	free(temp);
	
	return (result);
}

int	update_env_value(char *name, char *value, t_shell *shell)
{
	int		index;
	char	*new_str;

	//printf("Debug: Updating env value: %s=%s\n", name, value);
	if (!name || !value || !shell->envp)
	{
		//printf("Debug: Invalid parameters in update_env_value\n");
		return (1);
	}
	
	// Find the existing variable first
	index = find_env_index(name, shell->envp);
	//printf("Debug: Found index: %d\n", index);
	
	if (index < 0)
	{
		//printf("Debug: Environment variable not found\n");
		return (1);
	}

	// Create the new environment string
	new_str = create_env_string(name, value);
	if (!new_str)
	{
		//printf("Debug: Failed to create env string\n");
		return (1);
	}
	
	//printf("Debug: Replacing env string at index %d\n", index);
	//printf("Debug: Old string: '%s'\n", shell->envp[index]);
	//printf("Debug: New string: '%s'\n", new_str);
	
	// Replace the string directly without freeing the old one
	shell->envp[index] = new_str;
	
	//printf("Debug: Successfully updated env variable\n");
	return (0);
}

int	check_env_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 1)
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd("': No such file or directory", STDERR_FILENO);
		return (0);
	}
	return (1);
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

void	print_env_var(char *env_var)
{
	if (has_equals_sign(env_var))
		ft_putendl_fd(env_var, STDOUT_FILENO);
}

int	ft_env(char **args, t_shell *shell)
{
	int	i;

	if (!check_env_args(args))
		return (127);
	if (!shell->envp)
	{
		ft_putendl_fd("env: environment not available", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (shell->envp[i])
	{
		print_env_var(shell->envp[i]);
		i++;
	}
	return (0);
}