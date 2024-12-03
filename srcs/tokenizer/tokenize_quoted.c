/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/03 13:19:11 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to extract a quoted string from the input string.
It handles both single and double quotes.*/

char	*extract_quoted(char *input, int *pos, char quote_type)
{
	char	*content;
	int		i;

	if (!input || !pos || !quote_type)
		return (NULL);
	(*pos)++;
	content = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!content)
		return (NULL);
	i = 0;
	while (input[*pos])
	{
		if (input[*pos] == quote_type)
		{
			if (input[*pos + 1] == quote_type)
			{
				(*pos)++;
			}
			else
			{
				(*pos)++;
				break ;
			}
		}
		if (quote_type == '\'')
		{
			content[i++] = input[*pos];
		}
		else if (quote_type == '"' && input[*pos] == '\\'
			&& (input[*pos + 1] == '"' || input[*pos + 1] == '\\' 
				|| input[*pos + 1] == '$'))
		{
			(*pos)++;
			content[i++] = input[*pos];
		}
		else
		{
			content[i++] = input[*pos];
		}
		(*pos)++;
	}
	content[i] = '\0';
	return (content);
}

/*This function is used to tokenize a quoted strings in shell input. \ 
It handles both single and double quotes.*/
t_token	*tokenize_quoted_str(char *input, int *i, t_shell *shell)
{
	char		*value;
	char		*expanded;
	t_token		*token;
	char		quote_type;

	quote_type = input[*i];
	value = extract_quoted(input, i, quote_type);
	if (!value)
		return (cleanup_and_exit(ERR_QUOTE, NULL, 1, shell), NULL);
	if (quote_type == '\'')
	{
		expanded = ft_strdup(value);
	}
	else if (quote_type == '"' && value[0] != '\0')
	{
		expanded = expand_env_vars(value, shell);
		free(value);
		value = expanded;
	}
	else 
	{
		expanded = ft_strdup(value);
	}
	token = create_token(value, TOKEN_WORD);
	free(value);
	return (token);
}
