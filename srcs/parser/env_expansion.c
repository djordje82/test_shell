/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:12 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/13 12:15:54 by jadyar           ###   ########.fr       */
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

static char	*get_var_content(const char *str, int *i, t_shell *shell)
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

static char	*handle_dollar(const char *str, int *i, t_shell *shell)
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

char	*process_char(const char *str, int *i, char *result, t_shell *shell)
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

char	*expand_env_vars(const char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	if (!str || !shell)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		temp = process_char(str, &i, result, shell);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		result = temp;
	}
	return (result);
}
