/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:16 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/20 16:21:06 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(const char *input)
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
		ft_putendl_fd("minishell: syntax error: unclosed quotes",
			STDERR_FILENO);
		g_exit_status = 2;
		return (0);
	}
	return (1);
}

static int	check_invalid_redirection(const char *input, int pos)
{
	if (input[pos] == '>' && input[pos + 1] == '>' && input[pos + 2] == '>')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `>'",
			STDERR_FILENO);
		g_exit_status = 2;
		return (1);
	}
	if (input[pos] == '<' && input[pos + 1] == '<' && input[pos + 2] == '<')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `<'",
			STDERR_FILENO);
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

static int	check_consecutive_redirections(const char *input, int *pos)
{
	int	i;

	i = *pos;
	if (input[i] == '>' || input[i] == '<')
		i++;
	if (input[i] == '>' || input[i] == '<')
		i++;
	while (input[i] && ft_is_whitespace(input[i]))
		i++;
	if (input[i] == '>' || input[i] == '<')
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `>'",
			STDERR_FILENO);
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

/*
*probably should be in a separate file but fuck it...
see how it does without the null,pos,shell,input check
*/
t_token	*get_token_type(const char *input, int *pos, t_shell *shell)
{
	t_token_type	type;

	while (input[*pos] && ft_is_whitespace(input[*pos]))
		(*pos)++;
	type = get_operator_type(input[*pos]);
	if (type == TOKEN_PIPE || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
	{
		if (check_invalid_redirection(input, *pos) 
			|| check_consecutive_redirections(input, pos))
			return (NULL);
		if (input[*pos] == '>' && input[*pos + 1] == '>')
			return (tokenize_double_operator(input, pos, ">>", TOKEN_APPEND));
		if (input[*pos] == '<' && input[*pos + 1] == '<')
			return (tokenize_double_operator(input, pos, "<<", TOKEN_HEREDOC));
		return (tokenize_single_operator(input, pos));
	}
	if (type == TOKEN_SQUOTE || type == TOKEN_DQUOTE)
	{
		if (input[*pos + 1] == '\'' || input[*pos + 1] == '"')
			return (tokenize_adjacent_quotes(input, pos, shell));
		else
			return (tokenize_quoted_str((char *)input, pos, shell));
	}
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
		if (head && *input == '\0')
			head = create_token("", TOKEN_WORD);
	}
	return (head);
}
