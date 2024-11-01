/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 12:02:47 by dodordev          #+#    #+#             */
/*   Updated: 2023/12/22 14:14:44 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_x(unsigned long long hx, char input, int *count)
{
	char	*hx_b;
	int		i;

	if (input == 'x')
		hx_b = "0123456789abcdef";
	else
		hx_b = "0123456789ABCDEF";
	if (hx >= 16)
	{
		ft_print_x(hx / 16, input, count);
	}
	i = hx % 16;
	write(1, &hx_b[i], 1);
	(*count)++;
	return (*count);
}
