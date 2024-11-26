/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/26 13:57:58 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to create a token list for wildcard matches. \ 
It creates a token for each match and links them together in a linked list. \ 
It returns the head of the token list or NULL if there is an error.*/


/*This function is used to tokenize a word from the input string. \ 
It extracts the word, expands environment variables, and creates a token list \ 
for wildcard matches. It returns the token list or NULL if there is an error.*/
t_token	*tokenize_word(const char *input, int *i, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;
	//char	**matches;

	value = extract_word(input, i);
	if (!value)
		return (NULL);
	expanded = expand_env_vars(value, shell);
	free(value);
	if (!expanded)
		return (NULL);
	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
