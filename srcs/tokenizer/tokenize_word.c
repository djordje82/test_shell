/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 15:16:30 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize_word(const char *input, int *i, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;

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
