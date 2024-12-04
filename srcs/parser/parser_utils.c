/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:19 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:43:06 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**copy_existing_args(char **new_args, char **args, int *i)
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

static char	**create_new_array(char **args, int count)
{
	char	**new_args;

	(void)args;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	return (new_args);
}

// TO DO: SPLIT
static char	**add_cmd_argument(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	(void)args;
	if (!new_arg)
		return (args);
	i = 0;
	while (args && args[i])
		i++;
	new_args = create_new_array(args, i);
	if (!new_args)
	{
		ft_free_array((void **)args, -1);
		return (NULL);
	}
	i = 0;
	if (!copy_existing_args(new_args, args, &i))
		return (NULL);
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		ft_free_array((void **)new_args, i);
		return (NULL);
	}
	new_args[i + 1] = NULL;
	ft_free_array((void **)args, -1);
	return (new_args);
}

static char	*process_token_value(char *value, t_token_type type)
{
	char	*result;

	if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
	{
		if (ft_strlen(value) < 2)
			return (ft_strdup(""));
		result = ft_substr(value, 1, ft_strlen(value) - 2);
	}
	else
	{
		result = ft_strdup(value);
	}
	return (result);
}

int	parse_cmd_arguments(t_token **token, t_command *cmd)
{
	char	**new_args;
	char	*processed_value;

	if (!*token || !(*token) || !cmd)
		return (0);
	processed_value = process_token_value((*token)->value, (*token)->type);
	if (!processed_value)
		return (0);
	new_args = add_cmd_argument(cmd->args, processed_value);
	free(processed_value);
	if (!new_args)
		return (0);
	cmd->args = new_args;
	*token = (*token)->next;
	return (1);
}
