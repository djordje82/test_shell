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

char *extract_env_var_name(const char *str)
{
    int i;
    
    i = 0;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    return (ft_substr(str, 0, i));
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
                char *var_name = extract_env_var_name(&str[i + 1]);
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
