/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:16 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/28 15:20:17 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function retrieves the next token from the input string based on the current position. It identifies the type of shell character at the current position and handles different types of tokens accordingly. It returns the new token or NULL if there is an error.*/
t_token	*get_token_type(const char *input, int *pos, t_shell *shell)
{
	t_token_type	type;

	if (!input || !pos || !shell)
		return (NULL);
	if (!input[*pos])
		return (NULL);
	while (input[*pos] && ft_is_whitespace(input[*pos]))
		(*pos)++;
	type = get_operator_type(input[*pos]);
	if (type == TOKEN_PIPE || type == TOKEN_RDRCT_IN || type == TOKEN_RDRCT_OUT)
	{
		if (input[*pos] == '>' && input[*pos + 1] == '>')
			return (tokenize_double_operator(input, pos, ">>", TOKEN_APPEND));
		if (input[*pos] == '<' && input[*pos + 1] == '<')
			return (tokenize_double_operator(input, pos, "<<", TOKEN_HEREDOC));
		return (tokenize_single_operator(input, pos));
	if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
		return (tokenize_quoted_str((char *)input, pos, shell));
	return (tokenize_word(input, pos, shell));
	if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
		return (tokenize_quoted_str((char *)input, pos, shell));
	return (tokenize_word(input, pos, shell));
}

/*This function appends a new token to the end of a linked list of tokens. It updates the head and current pointers accordingly.*/
static void	add_token_to_list(t_token **head, t_token **current,
		t_token *new_token)
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

static int	check_quotes(const char *input)
{
	int		i;
	char	quote;
	int		escaped;

	i = 0;
	quote = 0;
	quote = 0;
	while (input[i])
	{
		escaped = (i > 0 && input[i - 1] == '\\');
		if ((input[i] == '\'' || input[i] == '"') && !quote && !escaped)
			quote = input[i];
			quote = input[i];
		else if (input[i] == quote && !escaped)
			quote = 0;
		i++;
	}
	return (!quote);
	return (!quote);
}

static t_token	*initialize_tokenization(const char *input, t_shell *shell)
{
	if (!input || !shell)
		return (NULL);
	if (!check_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		g_exit_status = 2;
		return (NULL);
	}
	return ((t_token *)1);
}

/*This function tokenizes the input string into a linked list of tokens. \ 
It handles various types of tokens,
including pipes, redirection operators, quoted strings,
and words. It returns the head of the token list or NULL if there is an error.*/

t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token *head;      // pointer to the head of the token list
	t_token *current;   // pointer to the current token in the list
	t_token *new_token; // pointer to the new token to be added to the list
	int pos;            // position in the input string
	if (!initialize_tokenization(input, shell))
		return (NULL);
	head = NULL;
	current = NULL;
	pos = 0;
	while (input[pos])
	{
		new_token = get_token_type(input, &pos, shell);
		if (!new_token)
			return (NULL);
		add_token_to_list(&head, &current, new_token);
		skip_whitespace(input, &pos);
		if (!input[pos])
			break ;
	}
	return (head);
}
