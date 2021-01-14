/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:36:24 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 12:08:50 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t dest_size)
{
	size_t	src_len;

	if (!dest || !src)
		return (0);
	src_len = ft_strlen(src);
	if (src_len < dest_size)
		ft_memcpy(dest, src, src_len + 1);
	else if (dest_size > 0)
	{
		ft_memcpy(dest, src, dest_size - 1);
		dest[dest_size - 1] = '\0';
	}
	return (src_len);
}
