/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:19 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:58:21 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**add_argument(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	if (!new_arg)
		return (args);
	//printf("Debug: add_argument called with new_arg: '%s'\n", new_arg);
	i = 0;
	while (args && args[i])
		i++;
	//printf("Debug: Current arg count: %d\n", i);
	
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
    {
        if (args)
            ft_free_array((void **)args, -1);
        return (NULL);
    }

	i = 0;
	while (args && args[i])
	{
		new_args[i] = ft_strdup(args[i]);
		if (!new_args[i])
		{
			//printf("Debug: Failed to duplicate arg %d\n", i);
			ft_free_array((void **)new_args, i);
			if (args)
				ft_free_array((void **)args, -1);
			return (NULL);
		}
		//printf("Debug: Copied arg[%d]: '%s'\n", i, new_args[i]);
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		//printf("Debug: Failed to duplicate new_arg\n");
		ft_free_array((void **)new_args, i);
		if (args)
			ft_free_array((void **)args, -1);
		return (NULL);
	}
	//printf("Debug: Added new arg[%d]: '%s'\n", i, new_args[i]);
	new_args[i + 1] = NULL;
	
	if (args)
	{
		//printf("Debug: Freeing old args array\n");
		ft_free_array((void **)args, -1);
	}
	//printf("Debug: Successfully created new args array\n");
	return (new_args);
}

static char *process_token_value(char *value, t_token_type type)
{
    char *result;
    
	//printf("Debug: Processing token value: '%s' of type %d\n", value, type);
	(void)type;
    /*if (type == TOKEN_QUOTE || type == TOKEN_DQUOTE)
    {
        // Remove surrounding quotes
        //result = ft_substr(value, 1, ft_strlen(value) - 2);
		result = ft_strdup(value + 1);
    }
    else
    {
        result = ft_strdup(value);
    }*///add comment out here in case you need to reactivate this function
    result = ft_strdup(value);
    //printf("Debug: Processed result: '%s'\n", result);

    return result;
}

int	parse_arguments(t_token **token, t_command *cmd)
{
	char    **new_args;
    char    *processed_value;
    
    //printf("Debug: Parsing argument token: '%s' of type %d\n", (*token)->value, (*token)->type);
    
    processed_value = process_token_value((*token)->value, (*token)->type);
    //processed_value = ft_strdup((*token)->value);
    if (!processed_value)
        return (0);
    
    //printf("Debug: Adding processed value: '%s'\n", processed_value);
    
    new_args = add_argument(cmd->args, processed_value);
    free(processed_value);
    
    if (!new_args)
        return (0);
    
    cmd->args = new_args;
    *token = (*token)->next;
    return (1);
}
