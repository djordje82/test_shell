#include "minishell.h"

/*This function is used to append a part of the input string to the result.*/
char	*append_word_part(char *result, const char *input, 
	int start, int len)
{
	char	*temp;
	char	*new_result;

	if (!result || !input)
		return (NULL);
	temp = ft_substr(input, start, len);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	new_result = ft_strjoin_free(result, temp);
	free(temp);
	return (new_result);
}

/*This function is used to append an unquoted part of the input string to the result.*/
static char	*append_unquoted_part(char *result, const char *input,
	int *start, int *len)
{
	if (!result || !input || !start || !len)
		return (NULL);
	if (*len > 0)
	{
		result = append_word_part(result, input, *start, *len);
		if (!result)
			return (NULL);
		*start += *len;
		*len = 0;
	}
	return (result);
}

/*This function is used to process a quoted segment of the input string. It appends the part of the string that is not quoted to the result.*/
char	*process_quoted_segment(char *result, const char *input,
	int *start, int *len)
{
	char	quote_type;
	int		quote_pos;
	char	*quoted_content;

	if (!result || !input || !start || !len)
		return (NULL);
	result = append_unquoted_part(result, input, start, len);
	if (!result)
		return (NULL);
	quote_type = input[*start + *len];
	quote_pos = *start + *len;
	quoted_content = extract_quoted((char *)input, &quote_pos, quote_type);
	if (!quoted_content)
	{
		free(result);
		return (NULL);
	}
	result = ft_strjoin_free(result, quoted_content);
	free(quoted_content);
	*len = quote_pos - *start - *len;
	*start += *len;
	return (result);
}

/*This function is used to process the content of a word in the input string. It handles both quoted and unquoted parts.*/
static char	*process_word_content(const char *input, int *start, int *len)
{
	char	*result;
	char	curr_char; //is used to store the current character in the input string

	if (!input || !start || !len)
		return (NULL);
	result = ft_strdup(""); //we need to initialize the result with an empty string
	if (!result)
		return (NULL);
	while (input[*start + *len])
	{
		curr_char = input[*start + *len]; //we set it to the position of the current character in the input string
		if (curr_char == '\'' || curr_char == '"') //if the current character is a quote, we process the quoted segment
		{
			result = process_quoted_segment(result, input, start, len);
			if (!result)
				return (NULL);
			continue ;
		}
		if (is_word_delimiter(curr_char)) //if the current character is a word delimiter, we break the loop
			break ;
		(*len)++; //we increment the length of the word if it's regular character
	}
	if (*len > 0)
		result = append_word_part(result, input, *start, *len); //we append the part of the word to the result
	return (result);
}

/*This function is used to extract a word from the input string. It handles both quoted and unquoted parts of the string.*/
char	*extract_word(const char *input, int *pos)
{
	char	*result;
	int		start;
	int		len;

	start = *pos; //we use the pointer to the position of the start of the word
	len = 0; //is used to count the number of characters in the word
	result = process_word_content(input, &start, &len);
	if (!result || !*result)
	{
		free(result);
		return (NULL);
	}
	*pos = start + len; // we update the pointer to the position of the end of the word
	return (result);
}