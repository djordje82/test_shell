/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:12 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:46:15 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value_expanded(char *name, t_shell *shell)
{
	char	*value;

	if (!name)
        return (ft_strdup(""));
    if (ft_strncmp(name, "?", 1) == 0)
        return (ft_itoa(shell->exit_status));
    
    value = get_env_value(name, shell);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

/*This function is used to get the content of a variable.*/
static char *get_var_content(char *str, int *i, t_shell *shell)
{
    char *var_name;
    char *value;

    if (is_whitespace(str[*i + 1]))
    {
        (*i)++;
        return (ft_strdup("$"));
    }

    var_name = extract_env_var_name(&str[*i + 1]);
    if (!var_name)
        return (ft_strdup(""));

    *i += ft_strlen(var_name) + 1;
    value = get_env_value_expanded(var_name, shell);
    free(var_name);
    return (value);
}

/*This function is used to expand the environment variables in the input string.*/
char	*expand_env_vars(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			temp = get_var_content(str, &i, shell);
		else
		{
			temp = ft_substr(str, i++, 1);
		}
		result = ft_strjoin_free(result, temp);
		free(temp);
	}
	return (result);
}
