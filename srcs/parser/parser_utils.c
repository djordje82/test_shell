/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:19 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/15 14:59:53 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_existing_args(char **new_args, char **args, int *i)
{
	while (args && args[*i])
	{
		new_args[*i] = ft_strdup(args[*i]);
		if (!new_args[*i])
		{
			ft_free_array((void **)new_args, *i);
			return (NULL);
		}
		(*i)++;
	}
	return (new_args);
}

char	**create_new_array(char **args, int count)
{
	char	**new_args;

	(void)args;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	while (count >= 0)
	{
		new_args[count] = NULL;
		count--;
	}
	return (new_args);
}

char	**add_new_argument(char **new_args, char *new_arg, int i)
{
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		ft_free_array((void **)new_args, i);
		return (NULL);
	}
	new_args[i + 1] = NULL;
	return (new_args);
}

char	**add_cmd_argument(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	if (!new_arg)
		return (args);
	i = ft_count_args(args);
	new_args = create_new_array(args, i);
	if (!new_args)
	{
		ft_free_array((void **)args, -1);
		return (NULL);
	}
	i = 0;
	if (!copy_existing_args(new_args, args, &i))
	{
		ft_free_array((void **)args, -1);
		return (NULL);
	}
	return (add_new_argument(new_args, new_arg, i));
}
