/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 12:27:01 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/09 16:35:35 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *src_tmp;
	unsigned char *dest_tmp;

	src_tmp = (unsigned char *)src;
	dest_tmp = (unsigned char *)dest;
	if (dest_tmp == src_tmp)
		return (dest);
	while (n-- > 0)
		*dest_tmp++ = *src_tmp++;
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	char	*concat;

	if (!s1 && !s2)
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	concat = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!concat)
		return (0);
	ft_memcpy(concat, s1, s1_len);
	ft_memcpy(concat + s1_len, s2, s2_len + 1);
	return (concat);
}
