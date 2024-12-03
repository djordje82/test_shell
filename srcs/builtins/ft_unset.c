/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:53:32 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/03 16:55:10 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_identifier_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

/*This function is used to remove an environment variable.*/
int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args || !shell)
		return (1);
	if (!args[1])
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!validate_env_var(args[i]))
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
