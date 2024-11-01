/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:08:02 by dodordev          #+#    #+#             */
/*   Updated: 2023/12/22 14:14:27 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_d(int nb, int *count)
{
	if (nb == -2147483648)
	{
		ft_print_c('-', count);
		ft_print_c('2', count);
		ft_print_d(147483648, count);
	}
	else if (nb < 0)
	{
		nb = -nb;
		ft_print_c('-', count);
		ft_print_d(nb, count);
	}
	else if (nb > 9)
	{
		ft_print_d(nb / 10, count);
		ft_print_d(nb % 10, count);
	}
	else
	{
		ft_print_c(nb + '0', count);
	}
}
