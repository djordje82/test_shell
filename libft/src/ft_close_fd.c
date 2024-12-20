/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:59:56 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/20 10:56:13 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_close_fd(int fd)
{
	if (fd >= 0)
	{
		close(fd);
		fd = -1;
	}
	else 
	{
		ft_putstr_fd("Error: Invalid file descriptor\n", STDERR_FILENO);
	}
}
