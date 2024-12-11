/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:56:04 by dodordev          #+#    #+#             */
/*   Updated: 2023/11/07 15:37:46 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] == s2[i])
			i++;
		else
			return (s1[i] - s2[i]);
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	char	s1[] = "Hello World!";
	char	s2[] = "No!";

	printf("ft_strcmp:\n%d\n", ft_strcmp(s1, s2));
	printf("strcmp:\n%d\n", strcmp(s1, s2));

	return (0);
}
*/
