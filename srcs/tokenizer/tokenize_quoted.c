/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:46:38 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to extract a quoted string from the input string. It handles both single and double quotes.*/
char	*extract_quoted(char *input, int *pos, char quote_type)
{
	int		start;
	int		len;
	char	*content;

	if (!input || !pos)
		return (NULL);
	(*pos)++;
	start = *pos;
	len = 0;
	while (input[*pos] && input[*pos] != quote_type)
	{
		len++;
		(*pos)++;
	}
	if (!input[*pos])
		return (NULL);
	content = ft_substr(input, start, len);
	(*pos)++;
	return (content);
}

/*This function is used to tokenize a quoted strings in shell input. It handles both single and double quotes.*/
t_token *tokenize_quoted_str(char *input, int *i, t_shell *shell)
{
	char		*value;
	char		*expanded;
	t_token		*token;
	char		quote_type;

	quote_type = input[*i];
	value = extract_quoted(input, i, quote_type);
	if (!value)
		return (cleanup_and_exit(ERR_QUOTE, NULL, 1, shell), NULL);
	if (quote_type == '"')
	{
		expanded = expand_env_vars(value, shell);
		free(value);
		value = expanded;
	}
	token = create_token(value, TOKEN_WORD);
	free(value);
	if (input[*i] == quote_type)
		(*i)++;
	return (token);
}