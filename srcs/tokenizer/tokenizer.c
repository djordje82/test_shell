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

/*This function retrieves the next token from the input string based on the current position. It identifies the type of shell character at the current position and handles different types of tokens accordingly. It returns the new token or NULL if there is an error.*/
t_token *get_next_token(const char *input, int *pos, t_shell *shell)
{
    t_char_type	type;

	if (!input || !pos || !shell)
		return (NULL);
	if (!input[*pos])
		return (NULL);
	while (input[*pos] && is_whitespace(input[*pos]))
		(*pos)++;
	type = find_special_chars(input[*pos]);
	if (type == CHAR_PIPE)
        return (tokenize_operator(input, pos));
    if (type == CHAR_REDIR_IN || type == CHAR_REDIR_OUT)
        return (tokenize_operator(input, pos));
    if (type == CHAR_SQUOTE || type == CHAR_DQUOTE)
        return (tokenize_quoted_str((char *)input, pos, shell));
    /* if (type == CHAR_SEMICOLON) // 
    {
        (*pos)++;
        return (NULL);
    } */
    return (tokenize_word(input, pos, shell));
}

/*This function appends a new token to the end of a linked list of tokens. It updates the head and current pointers accordingly.*/
static void	append_token(t_token **head, t_token **current, t_token *new_token)
{
	if (!new_token)
		return ;
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

/*Error handler for tokenization errors. It frees the token list and returns NULL.*/
static t_token	*handle_token_error(t_token *head)
{
	if (head)
		cleanup_token_list(head);
	return (NULL);
}

/*This function tokenizes the input string into a linked list of tokens. It handles various types of tokens, including pipes, redirection operators, quoted strings, and words. It returns the head of the token list or NULL if there is an error.*/
t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token	*head; //pointer to the head of the token list
	t_token	*current; //pointer to the current token in the list
	t_token	*new_token; //pointer to the new token to be added to the list
	int		pos; //position in the input string

	if (!input || !shell)
		return (NULL);
	head = NULL;
	current = NULL;
	pos = 0;
	while (input[pos])
	{
		new_token = get_next_token(input, &pos, shell);
		if (!new_token)
			return (handle_token_error(head));
		append_token(&head, &current, new_token);
		skip_whitespace(input, &pos);
		if (!input[pos])
			break ;
	}
	return (head);
}
