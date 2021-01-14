/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 11:01:15 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 10:58:43 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *lst_tmp;

	if (!*lst)
		*lst = new;
	else
	{
		lst_tmp = *lst;
		while (lst_tmp->next)
			lst_tmp = lst_tmp->next;
		lst_tmp->next = new;
	}
}
