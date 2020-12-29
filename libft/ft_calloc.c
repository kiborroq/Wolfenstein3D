/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:14:02 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/02 16:14:49 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void *buf;

	if (!nmemb || !size)
	{
		nmemb = 1;
		size = 1;
	}
	buf = (void *)malloc(nmemb * size);
	if (!buf)
		return (0);
	ft_bzero(buf, nmemb * size);
	return (buf);
}
