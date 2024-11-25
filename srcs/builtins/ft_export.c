/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:53:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/25 14:06:51 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

void	print_sorted_env(char **envp)
{
	int		i;
	int		j;
	char	*temp;
	char	**sorted;

	sorted = copy_env(envp);
	if (!sorted)
		return ;
	i = 0;
	while (sorted[i])
	{
		j = i + 1;
		while (sorted[j])
		{
			if (ft_strncmp(sorted[i], sorted[j], ft_strlen(sorted[i])) > 0)
			{
				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (sorted[i])
		print_exported_var(sorted[i++]);
	ft_free_array((void **)sorted, -1);
}

void	print_exported_var(char *var)
{
	int	i;
	int	found_equals;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	i = 0;
	found_equals = 0;
	while (var[i])
	{
		ft_putchar_fd(var[i], STDOUT_FILENO);
		if (var[i] == '=')
		{
			ft_putchar_fd('"', STDOUT_FILENO);
			found_equals = 1;
		}
		i++;
	}
	if (found_equals)
		ft_putchar_fd('"', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
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

int	get_env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	*get_var_value(char *arg)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
		return (ft_strdup(""));
	return (ft_strdup(equals + 1));
}

int	update_existing_var(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*equals;
	int		index;
	char	*new_value;

	// printf("Debug: Updating existing var: %s\n", arg);
	equals = ft_strchr(arg, '=');
	if (!equals)
		return (0); // Not an assignment
	// Get variable name
	name = ft_substr(arg, 0, equals - arg);
	if (!name)
		return (0);
	// Get variable value
	value = ft_strdup(equals + 1);
	if (!value)
	{
		free(name);
		return (0);
	}
	// Update the variable
	index = find_env_index(name, shell->envp);
	if (index >= 0)
	{
		// printf("Debug: Found existing variable at index %d\n", index);
		new_value = create_env_string(name, value);
		if (new_value)
		{
			free(shell->envp[index]);
			shell->envp[index] = new_value;
			// printf("Debug: Updated variable: %s\n", new_value);
		}
		free(name);
		free(value);
		return (1);
	}
	free(name);
	free(value);
	return (0);
}

int	add_new_var(char *arg, t_shell *shell)
{
	char	**new_env;
	int		size;
	int		i;

	size = get_env_size(shell->envp);
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

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		status;
	char	*name;
	char	*value;

	if (!args[1])
	{
		print_sorted_env(shell->envp);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '=' || !validate_env_var(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
		{
			name = extract_env_var_name(args[i]);
			value = get_var_value(args[i]);
			if (!update_env_value(name, value, shell))
				status = 1;
			free(name);
			free(value);
		}
		i++;
	}
	return (status);
}
