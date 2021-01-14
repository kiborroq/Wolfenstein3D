/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 12:08:31 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:35:01 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*clean_all(void *content, t_list **lst, void (*del)(void *))
{
	if (content)
		del(content);
	ft_lstclear(lst, del);
	return (0);
}

t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
							void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*curr;
	t_list	*new;
	void	*content;

	if (!lst || !f || !del)
		return (0);
	new_lst = 0;
	while (lst)
	{
		content = f(lst->content);
		if (!content)
			return (clean_all(content, &new_lst, del));
		new = ft_lstnew(content);
		if (!new)
			return (clean_all(content, &new_lst, del));
		if (!new_lst)
			new_lst = new;
		else
			curr->next = new;
		curr = new;
		lst = lst->next;
	}
	return (new_lst);
}
