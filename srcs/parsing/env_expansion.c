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

/*This function is used to get the content of a variable.*/
static char	*get_var_content(char *str, int *i, t_shell *shell)
{
	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->exit_status));
	}
	if (is_whitespace(str[*i + 1]))
	{
		*i += 1;
		return (ft_strdup("$"));
	}
	char	*var_name = extract_env_var_name(&str[*i + 1]);
	char	*var_value = get_env_value(var_name, shell);
	*i += ft_strlen(var_name) + 1;
	free(var_name);
	return (ft_strdup(var_value ? var_value : ""));
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
