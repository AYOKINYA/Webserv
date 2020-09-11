/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 20:12:31 by jkang             #+#    #+#             */
/*   Updated: 2020/04/10 12:37:31 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_abs(long long n)
{
	if (n < 0)
		n = n * -1;
	return (n);
}

long long	ft_len(long long n)
{
	int len;

	len = 0;
	if (n <= 0)
		++len;
	while (n != 0)
	{
		n = n / 10;
		++len;
	}
	return (len);
}

char		*ft_itoa(int n)
{
	char		*res;
	size_t		len;
	long		nbr;

	nbr = n;
	len = ft_len(nbr);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	res[len] = '\0';
	if (nbr < 0)
	{
		res[0] = '-';
		nbr = ft_abs(nbr);
	}
	if (nbr == 0)
		res[0] = '0';
	while (nbr != 0)
	{
		--len;
		res[len] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	return (res);
}
