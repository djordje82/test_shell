/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:58:51 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/20 13:03:54 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(const char *value, t_token_type type)
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

void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] && (input[*pos] == ' ' || \
		input[*pos] == '\t' || input[*pos] == '\r' || input[*pos] == '\n'))
		(*pos)++;
}

int	initialize_tokenization(const char *input, t_shell *shell)
{
	if (!input || !shell || !*input)
		return (0);
	if (!check_quotes(input))
	{
		g_exit_status = 2;
		return (0);
	}
	return (1);
}

void	add_token_to_list(t_token **head, t_token **current, t_token *new_token)
{
	if (!new_token)
	{
		ft_putendl_fd("Error: Null token", STDERR_FILENO);
		return ;
	}
	if (!*head)
	{
		*head = new_token;
		*current = new_token;
	}
	else
	{
		(*current)->next = new_token;
		*current = new_token;
	}
}
