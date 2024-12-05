/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_arguments.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:12:17 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/05 16:24:11 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
