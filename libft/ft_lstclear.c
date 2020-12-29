/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 11:03:41 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/04 11:05:20 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *lst_tmp;
	t_list *del_elem;

	if (!lst || !del)
		return ;
	lst_tmp = *lst;
	while (lst_tmp)
	{
		del_elem = lst_tmp;
		lst_tmp = lst_tmp->next;
		ft_lstdelone(del_elem, del);
	}
	*lst = 0;
}
