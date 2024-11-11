#include "minishell.h"

t_token *handle_word(const char *input, int *i, t_shell *shell)
{
    char    *value;
    char    *expanded;
    t_token *token;

    //printf("Debug: handle_word at position %d: '%c'\n", *i, input[*i]);
    value = extract_word(input, i);
    if (!value)
        return (NULL);
    
    //printf("Debug: Extracted word: '%s'\n", value);
    // Expand environment variables
    expanded = expand_env_vars(value, shell);
    free(value);
    if (!expanded)
        return (NULL);
    
    //printf("Debug: Expanded value: '%s'\n", expanded);
    token = create_token(expanded, TOKEN_WORD);
    free(expanded);
    
    // Don't decrement position here - let extract_word handle the position
    return (token);
}

t_token *handle_pipe(const char *input, int *pos)
{
    t_token *token;
    (void)input;  // Silence unused parameter warning
    
    //printf("Debug: Handling pipe token at position %d\n", *pos);
    token = create_token("|", TOKEN_PIPE);
    if (!token)
        return NULL;
    (*pos)++; // Add this line to increment the position after handling pipe
    return token;
}

t_token *handle_quote(char *input, int *i, t_shell *shell)
{
    char    *value;
    t_token *token;
    char    quote_type;

    quote_type = input[*i];
    //printf("Debug: handle_quote: Processing quote at position %d\n", *i);
    //(*i)++;  // Skip opening quote
    value = extract_quoted(input, i, quote_type);
    if (!value)
        return (exit_error(ERR_QUOTE, NULL, 1, shell), NULL);

    // Create token with the content without quotes
    //printf("Debug: handle_quote: Extracted value: '%s'\n", value);
    token = create_token(value, (quote_type == '"') ? TOKEN_DQUOTE : TOKEN_QUOTE);
    free(value);
    
    if (input[*i] == quote_type)
        (*i)++;  // Skip closing quote
        
    return token;
}

t_token *get_next_token(const char *input, int *pos, t_shell *shell)
{
    while (input[*pos] && is_whitespace(input[*pos]))
        (*pos)++;
        
    if (!input[*pos])
        return (NULL);
        
    t_char_type type = get_char_type(input[*pos]);
    
    if (type == CHAR_PIPE)
        return (handle_pipe(input, pos));
    if (type == CHAR_REDIR_IN || type == CHAR_REDIR_OUT)
        return (handle_operator(input, pos));
    if (type == CHAR_QUOTE || type == CHAR_DQUOTE)
        return (handle_quote((char *)input, pos, shell));
    if (type == CHAR_SEMICOLON)
    {
        (*pos)++;  // Skip semicolon for now
        return (NULL);
    }
    return (handle_word(input, pos, shell));
}

/*t_token *get_next_token(const char *input, int *pos, t_shell *shell)
{
    while (input[*pos] && is_whitespace(input[*pos]))
        (*pos)++;
    
    if (!input[*pos])
        return NULL;
        
    //printf("Debug: Processing character '%c' at position %d\n", input[*pos], *pos);

    // Handle special characters
    if (input[*pos] == ';')
    {
        //printf("Debug: Found semicolon - not supported\n");
        return NULL;  // Or handle error differently
    }

    if (is_operator(input[*pos]))  // Added this condition here CHECK IF IT WORKS
        return handle_operator(input, pos);
    else if (input[*pos] == '|')
        return handle_pipe(input, pos);
    else if (input[*pos] == '\'' || input[*pos] == '\"')
        return handle_quote((char *)input, pos, shell);  // Cast is safe here as we don't modify input
    else
        return handle_word(input, pos, shell);
}*/


t_token *tokenize_input(const char *input, t_shell *shell)
{
    t_token *head = NULL;
    t_token *current = NULL;
    int pos = 0;
    
    //printf("Debug: Starting tokenization of: '%s'\n", input);
    
    while (input[pos])
    {
        t_token *new_token = get_next_token(input, &pos, shell);
        if (!new_token) //removed break; here
        {
            if (head)
                free_tokens(head);  // Clean up all tokens if error occurs
            return NULL;
        }
        
        //printf("Debug: New token created: '%s' of type %d\n", new_token->value, new_token->type);

        if (!head)
        {
            head = new_token;
            current = new_token;
        }
        else
        {
            current->next = new_token;
            current = new_token;
        }
        // Skip any whitespace after the token
        while (input[pos] && is_whitespace(input[pos]))
            pos++;
            
        if (!input[pos])  // If we've reached the end
            break;
    }
    
    return head;
}
