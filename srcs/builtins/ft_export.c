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

static int process_export_var(char *arg, t_shell *shell)
{
    char    *name;
    char    *value;

    name = extract_env_var_name(arg);
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

static int validate_and_process(char *arg, t_shell *shell)
{
    if (arg[0] == '=' || !validate_env_var(arg))
    {
        print_export_error(arg);
        return (1);
    }
    return (process_export_var(arg, shell));
}

int ft_export(char **args, t_shell *shell)
{
    int     i;
    int     status;

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

/* int	ft_export(char **args, t_shell *shell)
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
			print_export_error(args[i]);
			status = 1;
		}
		else
		{
			name = extract_env_var_name(args[i]);
			value = extract_env_var_value(args[i]);
			if (!update_env_value(name, value, shell))
				status = 1;
			free(name);
			free(value);
		}
		i++;
	}
	return (status);
} */
