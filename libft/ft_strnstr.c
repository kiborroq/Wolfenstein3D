/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:39:26 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 11:50:56 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t n)
{
	size_t lit_len;
	size_t big_len;

	lit_len = ft_strlen(little);
	if (lit_len == 0)
		return ((char *)big);
	big_len = ft_strlen(big);
	if (lit_len > big_len || big_len == 0)
		return (0);
	while (*big && n > 0)
	{
		if (lit_len > n)
			return (0);
		if (*big == *little)
			if (!ft_strncmp(big, little, lit_len))
				return ((char *)big);
		n--;
		big++;
	}
	return (0);
}
