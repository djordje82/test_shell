/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:47:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:08:10 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env_string(char *name, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

int	update_env_value(char *name, char *value, t_shell *shell)
{
	int index;
	char *new_str;

	if (!name || !value || !shell->envp)
		return (1);
	index = find_env_index(name, shell->envp);
	if (index < 0)
		return (1);
	new_str = create_env_string(name, value);
	if (!new_str)
		return (1);
	free(shell->envp[index]);
	shell->envp[index] = new_str;
	return (0);
}