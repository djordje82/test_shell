/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:53:32 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/17 11:53:51 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_identifier_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	remove_env_var(char *name, t_shell *shell)
{
	int		i;
	int		j;
	int		name_len;
	//char	**new_env;

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

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_shell_var(args[i]))
		{
			print_identifier_error(args[i]);
			status = 1;
		}
		else
			remove_env_var(args[i], shell);
		i++;
	}
	return (status);
}