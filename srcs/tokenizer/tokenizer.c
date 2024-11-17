/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:16 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:59:18 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *get_next_token(const char *input, int *pos, t_shell *shell)
{
    while (input[*pos] && is_whitespace(input[*pos]))
        (*pos)++;
        
    if (!input[*pos])
        return (NULL);
        
    t_char_type type = identify_shell_char(input[*pos]);
    
    if (type == CHAR_PIPE)
        return (handle_pipe(input, pos));
    if (type == CHAR_REDIR_IN || type == CHAR_REDIR_OUT)
        return (tokenize_operator(input, pos));
    if (type == CHAR_QUOTE || type == CHAR_DQUOTE)
        return (tokenize_quoted_str((char *)input, pos, shell));
    if (type == CHAR_SEMICOLON)
    {
        (*pos)++;  // Skip semicolon for now
        return (NULL);
    }
    return (tokenize_word(input, pos, shell));
}

static void	add_token_to_list(t_token **head, t_token **current, t_token *new_token)
{
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

static void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] && is_whitespace(input[*pos]))
		(*pos)++;
}

static t_token	*handle_token_error(t_token *head)
{
	if (head)
		cleanup_token_list(head);
	return (NULL);
}

t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		pos;

	head = NULL;
	current = NULL;
	pos = 0;
	while (input[pos])
	{
		new_token = get_next_token(input, &pos, shell);
		if (!new_token)
			return (handle_token_error(head));
		add_token_to_list(&head, &current, new_token);
		skip_whitespace(input, &pos);
		if (!input[pos])
			break ;
	}
	return (head);
}
