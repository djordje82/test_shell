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

#include "minishell.h"

/*int	is_valid_identifier(char *name)
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
}*/

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

	i = 0;
	while (envp[i])
		i++;
	temp = NULL;
	i = 0;
	while (envp[i])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strncmp(envp[i], envp[j], ft_strlen(envp[i])) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
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

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
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

char	*get_var_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	return (name);
}

int	update_existing_var(char *arg, t_shell *shell)
{
	char	*name;
	int		i;
	int		name_len;

	name = get_var_name(arg);
	if (!name)
		return (0);
	name_len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, name_len) == 0
			&& (shell->envp[i][name_len] == '='
			|| shell->envp[i][name_len] == '\0'))
		{
			free(shell->envp[i]);
			shell->envp[i] = ft_strdup(arg);
			free(name);
			return (1);
		}
		i++;
	}
	free(name);
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
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_env;
	return (1);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		status;
	char	**sorted_env;

	if (!args[1])
	{
		sorted_env = copy_env(shell->envp);
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