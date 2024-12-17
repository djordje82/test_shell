/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:53:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 12:40:49 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_export_var(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;

	name = extract_env_var_name(arg);
	if (!name)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		return (1);
	}
	value = extract_env_var_value(arg);
	if (!update_env_value(name, value, shell))
	{
		free(name);
		free(value);
		return (1);
	}
	free(name);
	free(value);
	return (0);
}

static int	validate_and_process(char *arg, t_shell *shell)
{
	if (!arg || arg[0] == '=' || !validate_env_var(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	return (process_export_var(arg, shell));
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args[1])
	{
		print_sorted_env(shell->envp);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (validate_and_process(args[i], shell))
			status = 1;
		i++;
	}
	return (status);
}
