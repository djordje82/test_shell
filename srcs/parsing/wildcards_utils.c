/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:46:24 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 17:46:26 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_star(const char *str, const char *pattern)
{
	while (*pattern == '*')
		pattern++;
	if (*pattern == '\0')
		return (1);
	while (*str)
	{
		if (match_pattern(str, pattern))
			return (1);
		str++;
	}
	return (match_pattern(str, pattern));
}

int	match_pattern(const char *str, const char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
		return (match_star(str, pattern));
	if (*str && (*pattern == '?' || *pattern == *str))
		return (match_pattern(str + 1, pattern + 1));
	return (0);
}

int	is_valid_match(char *name, char *pattern)
{
	return (name[0] != '.' && match_pattern(name, pattern));
}

/*This function is used to initialize the result array.*/
char	**init_result_array(size_t count, DIR **dir)
{
	char	**result;

	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	*dir = opendir(".");
	if (!*dir)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

/*This function is used to count the number of matches for a given pattern.*/
size_t	count_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	while ((entry = readdir(dir)))
	{
		if (is_valid_match(entry->d_name, pattern))
			count++;
	}
	closedir(dir);
	return (count);
}