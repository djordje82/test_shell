/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:20:55 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/24 13:21:11 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_array(void **array, int len)
{
	int	i;

	if (!array)
		return ;
	if (len < 0)
		free_no_len(array);
	else
	{
		i = 0;
		while (i < len)
		{
			if (array[i])
			{
				free(array[i]);
				array[i] = NULL;
			}
			i++;
		}
	}
	free(array);
	array = NULL;
}