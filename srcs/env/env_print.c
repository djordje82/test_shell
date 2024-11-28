/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:12:52 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 15:44:32 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env_array(char **sorted)
{
	int		i;
	int		j;
	char	*temp;

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
}

void	print_sorted_env(char **envp)
{
	int		i;
	char	**sorted;

	sorted = copy_env(envp);
	if (!sorted)
		return ;
	sort_env_array(sorted);
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
