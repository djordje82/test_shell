/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:16 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/15 17:00:51 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_quotes(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"') && !quote)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", \
			STDERR_FILENO);
		g_exit_status = 2;
		return (0);
	}
	return (1);
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

void	add_token_to_list(t_token **head, t_token **current, \
		t_token *new_token)
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

t_token	*get_token_type(const char *input, int *pos, t_shell *shell)
{
	t_token_type	type;

	if (!input || !pos || !shell || !input[*pos])
		return (NULL);
	while (input[*pos] && ft_is_whitespace(input[*pos]))
		(*pos)++;
	type = get_operator_type(input[*pos]);
	if (type == TOKEN_PIPE || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
	{
		if (input[*pos] == '>' && input[*pos + 1] == '>')
			return (tokenize_double_operator(input, pos, ">>", TOKEN_APPEND));
		if (input[*pos] == '<' && input[*pos + 1] == '<')
			return (tokenize_double_operator(input, pos, "<<", TOKEN_HEREDOC));
		if (input[*pos] == '\'' || input[*pos] == '"')
			return (tokenize_adjacent_quotes(input, pos, shell));
		return (tokenize_single_operator(input, pos));
	}
	if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
		return (tokenize_quoted_str(input, pos, shell));
	return (tokenize_word(input, pos, shell));
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
	if (!initialize_tokenization(input, shell))
		return (NULL);
	while (input[pos])
	{
		new_token = get_token_type(input, &pos, shell);
		if (!new_token)
		{
			cleanup_token_list(head);
			return (NULL);
		}
		add_token_to_list(&head, &current, new_token);
		skip_whitespace(input, &pos);
		if (!head && *input == '\0')
			head = create_token("", TOKEN_WORD);
	}
	return (head);
}
