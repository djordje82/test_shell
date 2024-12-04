/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 17:10:03 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TO DO: SPLIT
char	*extract_quoted(char *input, int *pos, char quote_type)
{
	char	*content;
	int		i;

	i = 0;
	if (!input || !pos)
		return (NULL);
	(*pos)++;
	content = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!content)
		return (NULL);
	while (input[*pos])
	{
		if (input[*pos] == quote_type)
		{
			if (input[*pos + 1] == quote_type)
			{
				(*pos)++;
				continue ;
			}
			(*pos)++;
			break ;
		}
		if (quote_type == '\'')
		{
			content[i++] = input[*pos];
		}
		else if (quote_type == '"' && input[*pos] == '\\' && (input[*pos
					+ 1] == '"' || input[*pos + 1] == '\\' || input[*pos
					+ 1] == '$'))
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

// TO DO: SPLIT
t_token	*tokenize_quoted_str(char *input, int *i, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;
	char	quote_type;

	quote_type = input[*i];
	value = extract_quoted(input, i, quote_type);
	if (!value)
		return (cleanup_and_exit(ERR_QUOTE, NULL, 1, shell), NULL);
	if (quote_type == '\'')
	{
		expanded = ft_strdup(value);
	}
	else if (quote_type == '"' && *value)
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
