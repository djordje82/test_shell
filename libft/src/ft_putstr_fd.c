/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 14:14:04 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/26 11:33:43 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>

void	ft_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		if (write(fd, s, 1) == -1)
		{
			if (errno == EPIPE)
				return ;
			write (1, "write error", 11);
		}
		s++;
	}
}
