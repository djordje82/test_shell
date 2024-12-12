/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quoted.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:36 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/12 18:12:21 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_quoted_content(const char *input, int *start, int *len)
{
	char	quote_type;
	char	*content;
	int		i;

	quote_type = input[*start];
	content = malloc(ft_strlen(input) + 1);
	i = 0;
	if (!content)
		return (NULL);
	(*start)++;
	while (input[*start])
	{
		if (input[*start] == quote_type)
		{
			(*start)++;
			break ;
		}
		if (quote_type == '"' && input[*start] == '\\' && (input[*start
					+ 1] == '"' || input[*start + 1] == '$'))
			(*start)++;
		content[i++] = input[(*start)++];
	}
	content[i] = '\0';
	*len = i;
	return (content);
}

char	*extract_quoted(const char *input, int *pos, char quote_type)
{
	char	*content;
	int		len;

	len = 0;
	if (!input || !pos || input[*pos] != quote_type)
		return (NULL);
	content = process_quoted_content(input, pos, &len);
	return (content);
}

t_token	*tokenize_quoted_str(const char *input, int *pos, t_shell *shell)
{
	char	*value;
	char	*expanded;
	t_token	*token;

	value = extract_quoted(input, pos, input[*pos]);
	if (!value)
		return (NULL);
	if (input[*pos - 1] == '"')
		expanded = expand_env_vars(value, shell);
	else
		expanded = ft_strdup(value);
	free(value);
	token = create_token(expanded, TOKEN_WORD);
	free(expanded);
	return (token);
}
