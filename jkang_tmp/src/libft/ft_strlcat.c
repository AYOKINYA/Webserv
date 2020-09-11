/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 15:56:28 by jkang             #+#    #+#             */
/*   Updated: 2020/04/10 12:55:14 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dest_len;
	size_t	src_len;

	dest_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = 0;
	if (dstsize > dest_len + 1)
	{
		while (src[i] != '\0' && i < dstsize - dest_len - 1)
		{
			dst[i + dest_len] = src[i];
			++i;
		}
		dst[i + dest_len] = '\0';
	}
	if (dest_len > dstsize)
		return (src_len + dstsize);
	return (src_len + dest_len);
}
