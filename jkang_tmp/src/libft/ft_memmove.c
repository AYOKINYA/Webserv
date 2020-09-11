/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 22:29:19 by jkang             #+#    #+#             */
/*   Updated: 2020/04/10 12:48:02 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			n;
	unsigned char	*dest;
	unsigned char	*source;

	dest = (unsigned char *)dst;
	source = (unsigned char *)src;
	n = len;
	if (src <= dst)
	{
		while (n--)
			*(dest + n) = *(source + n);
	}
	else
	{
		while (len--)
			*(dest++) = *(source++);
	}
	return (dst);
}
