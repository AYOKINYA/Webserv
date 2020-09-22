/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 22:28:31 by jkang             #+#    #+#             */
/*   Updated: 2020/04/10 12:22:48 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*res;
	size_t	i;

	i = 0;
	if (!(res = malloc(size * count)))
		return (0);
	while (i < count * size)
	{
		res[i] = 0;
		++i;
	}
	return ((void *)res);
}
