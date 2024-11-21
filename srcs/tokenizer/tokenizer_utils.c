/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:51 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:58:53 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function creates a new token with the given value and type. It allocates memory for the token, duplicates the value, and sets the type. It returns the new token or NULL if there is an error.*/
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

/*This function determines the type of token based on the character at the current position. It returns the appropriate token type for the character.*/
t_token_type	get_token_type(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<')
		return (TOKEN_RDRCT_IN);
	if (c == '>')
		return (TOKEN_RDRCT_OUT);
	if (c == '\'')
		return (TOKEN_QUOTE);
	if (c == '\"')
		return (TOKEN_DQUOTE);
	if (c == '$')
		return (TOKEN_ENV);
	return (TOKEN_WORD);
}

/*This function skips over any whitespace characters in the input string.*/
void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] && is_whitespace(input[*pos]))
		(*pos)++;
}