/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:46:38 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *handle_pipe(const char *input, int *pos)
{
    t_token *token;
    (void)input;

    token = create_token("|", TOKEN_PIPE);
    if (!token)
        return NULL;
    (*pos)++;
    return token;
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
		return (exit_error(ERR_QUOTE, NULL, 1, shell), NULL);
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
