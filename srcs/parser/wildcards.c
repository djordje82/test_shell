/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:59:24 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 17:59:26 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function is used to add a match to the result array.*/
static int	add_match_to_result(char **result, size_t i, char *name)
{
	result[i] = ft_strdup(name);
	if (!result[i])
		return (0);
	return (1);
}

/*This function is used to fill the result array with the matching entries.*/
static char	**fill_result_array(char *pattern, size_t count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	size_t			i;

	result = init_result_array(count, &dir);
	if (!result)
		return (NULL);
	i = 0;
	while ((entry = readdir(dir)))
	{
		if (is_valid_match(entry->d_name, pattern))
		{
			if (!add_match_to_result(result, i, entry->d_name))
			{
				ft_free_array((void **)result, i);
				closedir(dir);
				return (NULL);
			}
			i++;
		}
	}
	result[i] = NULL;
	closedir(dir);
	return (result);
}

/*This function is used to expand wildcards in the input pattern.*/
char	**expand_wildcards(char *pattern)
{
	size_t		count;

	if (!pattern)
		return (NULL);
	if (!ft_strchr(pattern, '*'))
		return (NULL);
	count = count_matches(pattern);
	if (count == 0)
		return (NULL);
	return (fill_result_array(pattern, count));
}
