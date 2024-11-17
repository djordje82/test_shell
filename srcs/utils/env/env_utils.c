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

int	check_env_args(char **args)
{
	int i;

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