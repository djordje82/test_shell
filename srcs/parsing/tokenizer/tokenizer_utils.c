#include "minishell.h"

char *get_var_name(const char *str)
{
    int i;
    
    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    return (ft_substr(str, 0, i));
}

// Joins two strings and frees the first one
char *ft_strjoin_free(char *s1, char *s2)
{
    char *result;
    
    result = ft_strjoin(s1, s2);
    free(s1);
    return (result);
}

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

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

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token_type	get_token_type(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<')
		return (TOKEN_REDIRECT_IN);
	if (c == '>')
		return (TOKEN_REDIRECT_OUT);
	if (c == '\'')
		return (TOKEN_QUOTE);
	if (c == '\"')
		return (TOKEN_DQUOTE);
	if (c == '$')
		return (TOKEN_ENV);
	return (TOKEN_WORD);
}

char *extract_word(const char *input, int *pos)
{
    //char *word;
    char *temp;
    char *result;
    int start = *pos;
    int len = 0;
    
    result = ft_strdup("");
    if (!result)
        return NULL;
    
    while (input[start + len])
    {
        // Handle quotes within word
        if (input[start + len] == '\'' || input[start + len] == '"')
        {
            // First, add any accumulated normal characters
            if (len > 0)
            {
                temp = ft_substr(input, start, len);
                result = ft_strjoin_free(result, temp);
                free(temp);
                start += len;
                len = 0;
            }
            
            // Extract quoted content
            char quote_type = input[start + len];
            int quote_pos = start + len;
            temp = extract_quoted((char *)input, &quote_pos, quote_type);
            if (!temp)
            {
                free(result);
                return NULL;
            }
            result = ft_strjoin_free(result, temp);
            free(temp);
            len = quote_pos - start - len;
            start += len;
            continue;
        }
        
        // Stop at whitespace or special characters (except quotes)
        if (is_whitespace(input[start + len]) || 
            (is_special_char(input[start + len]) && 
             input[start + len] != '\'' && 
             input[start + len] != '"'))
            break;
            
        len++;
    }
    
    // Add any remaining normal characters
    if (len > 0)
    {
        temp = ft_substr(input, start, len);
        result = ft_strjoin_free(result, temp);
        free(temp);
        start += len;
    }
    
    *pos = start;
    
    // If result is empty, free it and return NULL
    if (!*result)
    {
        free(result);
        return NULL;
    }
    
    return result;
}

char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;
	char	*result;

	//printf("Debug: Entering extract_quoted at position %d\n", *i);
	//printf("Debug: Looking for matching quote: '%c'\n", quote_type);
	//printf("Debug: Input string from current position: '%s'\n", input + *i);
	
	(*i)++;  // Skip opening quote at position 5
	start = *i;
	len = 0;
	// Debug the starting position
    //printf("Debug: extract_quoted starting at position %d with char '%c'\n", start, input[start]);
	
	while (input[*i] && input[*i] != quote_type)
	{
		//printf("Debug: Current char: '%c'\n", input[*i]);
		len++;
		(*i)++;
	}
	if (!input[*i])
	{
		//printf("Debug: No closing quote found\n");
		//(*i)--;  // Move back to the opening quote position
		return (NULL);
	}
	result = ft_substr(input, start, len);
	(*i)++; // Move past closing quote
	//*i = start + len;
	//printf("Debug: Extracted content: '%s'\n", result ? result : "NULL");
	return (result);
}

char *expand_env_vars(char *str, t_shell *shell)
{
    char *result;
    char *temp;
    int i;
    
    result = ft_strdup("");
    i = 0;
    
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            if (str[i + 1] == '?')
            {
                temp = ft_itoa(shell->exit_status);
                i += 2;
            }
			else if (is_whitespace(str[i + 1]))
			{
				temp = ft_strdup("$");
				i++;
			}
            else
            {
                char *var_name = get_var_name(&str[i + 1]);
                char *var_value = get_env_value(var_name, shell);
                temp = ft_strdup(var_value ? var_value : "");
                i += ft_strlen(var_name) + 1;
                free(var_name);
            }
            result = ft_strjoin_free(result, temp);
            free(temp);
        }
        else
        {
            temp = ft_substr(str, i, 1);
            result = ft_strjoin_free(result, temp);
            free(temp);
            i++;
        }
    }
    
    return result;
}

/*char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;
	char	*result;

	//printf("Debug: Entering extract_quoted at position %d\n", *i);
	(*i)++;
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote_type)
	{
		len++;
		(*i)++;
	}
	if (!input[*i])
	{
		//printf("Debug: No closing quote found\n");
		return (NULL);
	}
	result = ft_substr(input, start, len);
	//printf("Debug: Extracted content: '%s'\n", result ? result : "NULL");
	(*i)++;
	(*i)--;
	return (result);
}*/

/*char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;
	char	*result;

	(*i)++;  // moves past opening quote
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote_type)
	{
		len++;
		(*i)++;
	}
	if (!input[*i])
		return (NULL);
	result = ft_substr(input, start, len);
	(*i)++;  // move past closing quote
	(*i)--;  // decrement because tokenizer will increment
	return (result);
}*/

/*char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;

	(*i)++;
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote_type)
	{
		len++;
		(*i)++;
	}
	if (!input[*i])
		return (NULL);
	return (ft_substr(input, start, len));
}*/

/*char	*extract_quoted(char *input, int *i, char quote_type)
{
	int		start;
	int		len;
	char	*content;
	char	*quoted_str;

	(*i)++;
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote_type)
	{
		len++;
		(*i)++;
	}
	if (!input[*i])
		return (NULL);
	content = ft_substr(input, start, len);
	if (!content)
		return (NULL);
	// Preserve quotes in the token value
	quoted_str = ft_strjoin3(&quote_type, content, &quote_type);
	free(content);
	return (quoted_str);
}*/