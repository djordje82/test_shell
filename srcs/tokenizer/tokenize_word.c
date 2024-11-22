/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:46:48 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to create a token list for wildcard matches. \ 
It creates a token for each match and links them together in a linked list. \ 
It returns the head of the token list or NULL if there is an error.*/
static t_token	*create_wildcard_token_list(char **matches)
{
	t_token	*first;
	t_token	*current;
	int		i;

	if (!matches || !matches[0])
		return (NULL);
	first = create_token(matches[0], TOKEN_WORD);
	if (!first)
		return (NULL);
	current = first;
	i = 1;
	while (matches[i])
	{
		current->next = create_token(matches[i], TOKEN_WORD);
		if (!current->next)
		{
			cleanup_token_list(first);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	return (first);
}

/*This function is used to tokenize a word from the input string. \ 
It extracts the word, expands environment variables, and creates a token list \ 
for wildcard matches. It returns the token list or NULL if there is an error.*/
t_token	*tokenize_word(const char *input, int *i, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;
	char	**matches;

	value = extract_word(input, i);
	if (!value)
		return (NULL);
	expanded = expand_env_vars(value, shell);
	free(value);
	if (!expanded)
		return (NULL);
	matches = expand_wildcards(expanded);
	if (matches)
	{
		token = create_wildcard_token_list(matches);
		ft_free_array((void **)matches, -1);
		free(expanded);
		return (token);
	}
	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
