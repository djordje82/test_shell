/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:20:55 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/26 15:19:14 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_array(void **arr, int size)
{
	int i;

	if (!arr)
		return ;

	i = 0;
	if (size == -1)
	{
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
	}
	else
	{
		while (i < size)
		{
			free(arr[i]);
			i++;
		}
	}
	free(arr);
}