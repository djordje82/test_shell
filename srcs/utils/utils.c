/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:01:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:01:06 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_char_type get_char_type(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        return (CHAR_WHITESPACE);
    if (c == '|')
        return (CHAR_PIPE);
    if (c == '<')
        return (CHAR_REDIR_IN);
    if (c == '>')
        return (CHAR_REDIR_OUT);
    if (c == '\'')
        return (CHAR_QUOTE);
    if (c == '\"')
        return (CHAR_DQUOTE);
    if (c == ';')
        return (CHAR_SEMICOLON);
    //if (c == '$')
        //return (CHAR_ENV);
    return (CHAR_NORMAL);
}

// Replace multiple is_* functions with one function
int is_special_char(char c)
{
    t_char_type type; 
    
    type = get_char_type(c);
    return (type != CHAR_NORMAL);
}

// Keep only these essential helpers
int is_whitespace(char c)
{
    return (get_char_type(c) == CHAR_WHITESPACE);
}

int is_quote(char c)
{
    t_char_type type = get_char_type(c);
    return (type == CHAR_QUOTE || type == CHAR_DQUOTE);
}
