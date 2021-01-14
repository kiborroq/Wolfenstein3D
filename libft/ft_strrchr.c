/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:40:01 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 11:53:29 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int ch)
{
	size_t n;

	n = ft_strlen(str);
	if (!ch)
		return ((char *)(&str[n]));
	while (n-- > 0)
		if (str[n] == ch)
			return ((char *)(str + n));
	return (0);
}
