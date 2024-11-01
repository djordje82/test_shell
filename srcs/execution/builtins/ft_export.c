/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:53:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/17 11:53:22 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(name[0]))
		return (0);
	i = 0;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static void	print_sorted_env(char **env)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (env[i])
		i++;
	temp = NULL;
	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_exported_var(char *var)
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

static char	**copy_env(char **env)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
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

static int	get_env_size(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static char	*get_var_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	return (name);
}

static int	update_existing_var(char *arg, t_shell *shell)
{
	char	*name;
	int		i;
	int		name_len;

	name = get_var_name(arg);
	if (!name)
		return (0);
	name_len = ft_strlen(name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, name_len) == 0
			&& (shell->env[i][name_len] == '='
			|| shell->env[i][name_len] == '\0'))
		{
			free(shell->env[i]);
			shell->env[i] = ft_strdup(arg);
			free(name);
			return (1);
		}
		i++;
	}
	free(name);
	return (0);
}

static int	add_new_var(char *arg, t_shell *shell)
{
	char	**new_env;
	int		size;
	int		i;

	size = get_env_size(shell->env);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (0);
	i = 0;
	while (shell->env[i])
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (1);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		status;
	char	**sorted_env;

	if (!args[1])
	{
		sorted_env = copy_env(shell->env);
		if (!sorted_env)
			return (1);
		print_sorted_env(sorted_env);
		i = 0;
		while (sorted_env[i])
			print_exported_var(sorted_env[i++]);
		free_array((void **)sorted_env, -1);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_export_error(args[i]);
			status = 1;
		}
		else if (!update_existing_var(args[i], shell))
			if (!add_new_var(args[i], shell))
				status = 1;
		i++;
	}
	return (status);
}