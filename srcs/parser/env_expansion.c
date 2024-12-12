/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:12 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/12 18:12:13 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value_expanded(char *name, t_shell *shell)
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

static char	*get_var_content(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*value;

	if (ft_is_whitespace(str[*i + 1]))
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

static char	*handle_dollar(char *str, int *i, t_shell *shell)
{
	char	*temp;

	if (str[*i + 1] == '?')
	{
		temp = ft_itoa(shell->exit_status);
		*i += 2;
	}
	else if (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_')
	{
		temp = get_var_content(str, i, shell);
		if (!temp)
			temp = ft_strdup("");
	}
	else
	{
		temp = ft_strdup("$");
		(*i)++;
	}
	return (temp);
}

char	*process_char(char *str, int *i, char *result, t_shell *shell)
{
	char	*temp;

	if (str[*i] == '$')
	{
		temp = handle_dollar(str, i, shell);
	}
	else
	{
		temp = ft_substr(str, (*i)++, 1);
	}
	result = ft_strjoin_free(result, temp);
	free(temp);
	return (result);
}

char *expand_env_vars(const char *str, t_shell *shell) {
    char *result = ft_strdup("");
    char *temp;
    int i = 0;

    if (!str || !shell)
        return (NULL);
    while (str[i]) {
        if (str[i] == '\\' && (str[i + 1] == '$' || str[i + 1] == '\\')) {
            temp = ft_strdup("$");
            i += 2;
        } else if (str[i] == '$' && str[i + 1] == '?') {
            temp = ft_itoa(shell->exit_status);
            i += 2;
        } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
            char *var_name = extract_env_var_name(&str[i + 1]);
            if (var_name) {
                temp = ft_strdup(get_env_value(var_name, shell) ?: "");
                free(var_name);
                i += ft_strlen(var_name) + 1;
            } else {
                temp = ft_strdup("");
                i++;
            }
        } else {
            temp = ft_substr(str, i++, 1);
        }
        result = ft_strjoin_free(result, temp);
        free(temp);
    }
    return (result);
}