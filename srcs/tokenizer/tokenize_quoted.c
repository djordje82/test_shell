/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/27 11:59:19 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to extract a quoted string from the input string. \ 
It handles both single and double quotes.*/
char	*extract_quoted(char *input, int *pos, char quote_type)
{
	int		start;
	int		len;
	char	*content;
	int		i;

	if (!input || !pos)
		return (NULL);
	(*pos)++;  // Skip opening quote
	start = *pos;
	len = 0;
	i = 0;
	content = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!content)
		return (NULL);
	while (input[*pos] && input[*pos] != quote_type)
	{
		//printf("Current char: %c at pos: %d\n", input[*pos], *pos);
		if (input[*pos] == '\\' && quote_type == '"' 
			&& input[*pos + 1] != '\0')
		{
			(*pos)++;  // Move past backslash
			content[i++] = input[*pos];     // Count the escaped character
			//printf("Escaped char: %c at pos: %d\n", input[*pos], *pos);
		}
		else
			content[i++] = input[*pos];
		(*pos)++;
	}
	if (!input[*pos])
		return (free(content), NULL);
	//ft_strlcpy(content, input + start, len + 1);
	//printf("Final content: '%s'\n", content);
	content[i] = '\0';
	(*pos)++;  // Skip closing quote
	return (content);
}

/*This function is used to tokenize a quoted strings in shell input. \ 
It handles both single and double quotes.*/
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