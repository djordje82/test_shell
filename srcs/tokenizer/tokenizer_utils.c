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

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

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

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

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

