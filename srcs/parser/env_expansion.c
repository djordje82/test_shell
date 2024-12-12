/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:12 by dodordev          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/12/12 16:41:51 by jadyar           ###   ########.fr       */
=======
/*   Updated: 2024/12/11 18:06:46 by jadyar           ###   ########.fr       */
>>>>>>> 936e186 (quoted issue in tokenizer resolved)
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

char	*expand_env_vars(char *str, t_shell *shell)
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
		if (str[i] == '\\' && (str[i + 1] == '$' || str[i + 1] == '\\'))
		{
			temp = ft_substr(str, i + 1, 1);
			i += 2;
		}
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			temp = get_var_content(str, &i, shell);
			if (!temp)
				temp = ft_strdup("");
		}
		else if (str[i] == '$' && str[i + 1] == '?')
		{
			temp = ft_itoa(shell->exit_status);
			i += 2;
		}
		else if (str[i] == '$' && (!str[i + 1] || ft_is_whitespace(str[i + 1])))
		{
			temp = ft_strdup("$");
			i++;
		}
		else
			temp = ft_substr(str, i++, 1);
		result = ft_strjoin_free(result, temp);
		free(temp);
	}
	return (result);
}
