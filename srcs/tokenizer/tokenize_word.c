/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:45 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/06 15:51:30 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_word_part(char *result, const char *input, int start, int len)
{
	char	*temp;
	char	*new_result;

	if (!result || !input)
		return (NULL);
	temp = ft_substr(input, start, len);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	new_result = ft_strjoin_free(result, temp);
	if (!new_result)
		return (NULL);
	free(temp);
	return (new_result);
}

char	*append_unquoted_part(char *result, const char *input, int *start,
		int *len)
{
	if (!result || !input || !start || !len)
		return (NULL);
	if (*len > 0)
	{
		result = append_word_part(result, input, *start, *len);
		if (!result)
			return (NULL);
		*start += *len;
		*len = 0;
	}
	return (result);
}

char	*handle_quote_error(char *result)
{
	ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
	g_exit_status = 2;
	free(result);
	return (NULL);
}

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
