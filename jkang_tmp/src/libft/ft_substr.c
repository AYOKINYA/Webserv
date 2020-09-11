/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 20:12:47 by jkang             #+#    #+#             */
/*   Updated: 2020/06/06 23:03:33 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*temp;
	size_t			i;
	size_t			s_len;

	s_len = ft_strlen(s);
	if (s_len < start)
	{
		temp = (char *)malloc(sizeof(char) * 2);
		if (temp != 0)
			temp[0] = '\0';
		return (temp);
	}
	if (!(temp = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	i = 0;
	while (*s != '\0' && i < len)
	{
		temp[i] = *(s + start + i);
		++i;
	}
	temp[i] = '\0';
	return (temp);
}
