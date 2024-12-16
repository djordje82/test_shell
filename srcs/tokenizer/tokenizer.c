/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:16 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/16 19:00:55 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_invalid_redirection(const char *input, int pos)
{
    if (input[pos] == '>' && input[pos + 1] == '>' && input[pos + 2] == '>')
    {
        ft_putendl_fd("minishell: syntax error near unexpected token `>'", STDERR_FILENO);
        g_exit_status = 2;
        return (1);
    }
    if (input[pos] == '<' && input[pos + 1] == '<' && input[pos + 2] == '<')
    {
        ft_putendl_fd("minishell: syntax error near unexpected token `<'", STDERR_FILENO);
        g_exit_status = 2;
        return (1);
    }
    return (0);
}

static int check_consecutive_redirections(const char *input, int *pos)
{
    int i = *pos;

    // Skip current redirection operator
    if (input[i] == '>' || input[i] == '<')
        i++;
    if (input[i] == '>' || input[i] == '<')  // Skip second char for >> or 
        i++;
        
    // Skip whitespace
    while (input[i] && ft_is_whitespace(input[i]))
        i++;

    // Check if next non-whitespace char is a redirection
    if (input[i] == '>' || input[i] == '<')
    {
        ft_putendl_fd("minishell: syntax error near unexpected token `>'", STDERR_FILENO);
        g_exit_status = 2;
        return (1);
    }
    return (0);
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
		if (check_invalid_redirection(input, *pos))
            return (NULL);
		if (check_consecutive_redirections(input, pos))
            return (NULL);
		if (input[*pos] == '>' && input[*pos + 1] == '>')
			return (tokenize_double_operator(input, pos, ">>", TOKEN_APPEND));
		if (input[*pos] == '<' && input[*pos + 1] == '<')
			return (tokenize_double_operator(input, pos, "<<", TOKEN_HEREDOC));
		return (tokenize_single_operator(input, pos));
	}
	if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
		return (tokenize_quoted_str((char *)input, pos, shell));
	return (tokenize_word(input, pos, shell));
}

static void	add_token_to_list(t_token **head, t_token **current,
		t_token *new_token)
{
	if (!new_token)
	{
		ft_putendl_fd("Error: Null token", 2);
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
		else if (quote == '"' && input[i] == '\\' && input[i + 1] == '$')
			i++;
		else if (input[i] == quote)
			quote = 0;
		else if (!quote && (input[i] == '\'' || input[i] == '"'))
			return (0);
		i++;
	}
	return (!quote);
}

static int	initialize_tokenization(const char *input, t_shell *shell)
{
	if (!input || !shell || !*input)
		return (0);
	if (ft_strlen(input) == 0 || ft_is_whitespace(input[0]))
	{
		g_exit_status = 1;
		return (0);
	}
	if (!check_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		g_exit_status = 2;
		return (0);
	}
	return (1);
}

t_token	*tokenize_input(const char *input, t_shell *shell)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		pos;

	if (!initialize_tokenization(input, shell))
		return (NULL);
	head = NULL;
	current = NULL;
	pos = 0;
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
	}
	return (head);
}
