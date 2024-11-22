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

/*This function is used to copy existing arguments to the new array.*/
static char	**copy_existing_args(char **new_args, char **args, int *i)
{
	while (args && args[*i])
	{
		new_args[*i] = ft_strdup(args[*i]);
		if (!new_args[*i])
		{
			ft_free_array((void **)new_args, *i);
			ft_free_array((void **)args, -1);
			return (NULL);
		}
		(*i)++;
	}
	return (new_args);
}

/*This function is used to create a new array with the given count.*/
static char	**create_new_array(char **args, int count)
{
	char	**new_args;

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		ft_free_array((void **)args, -1);
		return (NULL);
	}
	return (new_args);
}

/*This function is used to add a new argument to the command's argument list.*/
static char	**add_cmd_argument(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	if (!new_arg)
		return (args);
	i = 0;
	while (args && args[i])
		i++;
	new_args = create_new_array(args, i);
	if (!new_args)
		return (NULL);
	i = 0;
	if (!copy_existing_args(new_args, args, &i))
		return (NULL);
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
	{
		ft_free_array((void **)new_args, i);
		ft_free_array((void **)args, -1);
		return (NULL);
	}
	new_args[i + 1] = NULL;
	ft_free_array((void **)args, -1);
	return (new_args);
}

/*This function is used to process the value of a token.*/
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

/*This function is used to parse the arguments of a command.*/
int	parse_cmd_arguments(t_token **token, t_command *cmd)
{
	char	**new_args;
	char	*processed_value;

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
