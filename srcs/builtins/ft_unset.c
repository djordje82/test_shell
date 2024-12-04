/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:53:32 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 18:11:29 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_identifier_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static int	process_argument(char *arg, t_shell *shell)
{
	if (!validate_env_var(arg))
	{
		print_identifier_error(arg);
		return (1);
	}
	else
	{
		if (remove_env_var(arg, shell) == -1)
		{
			print_identifier_error(arg);
			return (1);
		}
	}
	return (0);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args || !shell || !args[0])
		return (1);
	if (!args[1])
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		status |= process_argument(args[i], shell);
		i++;
	}
	return (status);
}
