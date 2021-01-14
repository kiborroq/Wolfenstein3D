/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:14:02 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 12:44:10 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void *mem;

	if (!nmemb || !size)
		mem = malloc(1);
	else
		mem = malloc(nmemb * size);
	if (!mem)
		return (0);
	ft_bzero(mem, nmemb * size);
	return (mem);
}
