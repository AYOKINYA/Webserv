/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 23:01:03 by jkang             #+#    #+#             */
/*   Updated: 2020/09/04 15:48:10 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_elem)
{
	t_list	*temp;

	if (lst == 0 || new_elem == 0)
		return ;
	if (*lst == 0)
		*lst = new_elem;
	else
	{
		temp = ft_lstlast(*lst);
		temp->next = new_elem;
	}
}
