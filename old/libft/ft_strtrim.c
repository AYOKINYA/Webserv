/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 20:12:42 by jkang             #+#    #+#             */
/*   Updated: 2020/05/29 21:49:22 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_is_set(const char c, char const *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (1);
		++set;
	}
	return (0);
}

char			*ft_strtrim(char *s1, char const *set)
{
	char	*trimmed_str;
	size_t	s1_len;

	s1_len = ft_strlen(s1);
	trimmed_str = (char *)malloc(sizeof(char) * (s1_len + 1));
	if (trimmed_str == 0)
		return (0);
	ft_memmove(trimmed_str, s1, s1_len + 1);
	while (ft_is_set(trimmed_str[0], set))
		ft_memmove(trimmed_str, trimmed_str + 1, ft_strlen(trimmed_str));
	while (ft_is_set(trimmed_str[ft_strlen(trimmed_str) - 1], set))
		trimmed_str[ft_strlen(trimmed_str) - 1] = '\0';
	free(s1);
	return (trimmed_str);
}
