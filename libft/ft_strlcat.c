/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:32:52 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 12:08:43 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dest_size)
{
	size_t	src_len;
	size_t	dest_len;

	src_len = ft_strlen((char *)src);
	dest_len = ft_strlen(dest);
	if (dest_size < dest_len + 1)
		return (dest_size + src_len);
	else if (dest_len + src_len < dest_size)
		ft_memcpy(dest + dest_len, src, src_len + 1);
	else if (dest_size > 0)
	{
		ft_memcpy(dest + dest_len, src, dest_size - dest_len - 1);
		dest[dest_size - 1] = '\0';
	}
	return (dest_len + src_len);
}
