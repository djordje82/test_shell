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

#include "mini_shell.c"

char	*get_env_value(char *name, t_shell *shell)
{
	int		i;
	int		len;
	char	*env_str;

	if (!name || !shell->env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
		{
			env_str = shell->env[i] + len + 1;
			return (env_str);
		}
		i++;
	}
	return (NULL);
}

static int	find_env_index(char *name, char **env)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	*create_env_string(char *name, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

int	update_env_value(char *name, char *value, t_shell *shell)
{
	int		index;
	char	*new_str;

	if (!name || !value || !shell->env)
		return (1);
	new_str = create_env_string(name, value);
	if (!new_str)
		return (1);
	index = find_env_index(name, shell->env);
	if (index >= 0)
	{
		free(shell->env[index]);
		shell->env[index] = new_str;
	}
	else
	{
		// TODO: Implement adding new environment variable
		free(new_str);
		return (1);
	}
	return (0);
}

static int	check_env_args(char **args)
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

static int	has_equals_sign(char *str)
{
	while (*str)
	{
		if (*str == '=')
			return (1);
		str++;
	}
	return (0);
}

static void	print_env_var(char *env_var)
{
	if (has_equals_sign(env_var))
		ft_putendl_fd(env_var, STDOUT_FILENO);
}

int	ft_env(char **args, t_shell *shell)
{
	int	i;

	if (!check_env_args(args))
		return (127);
	if (!shell->env)
	{
		ft_putendl_fd("env: environment not available", STDERR_FILENO);
		return (1);
	}
	i = 0;
	while (shell->env[i])
	{
		print_env_var(shell->env[i]);
		i++;
	}
	return (0);
}