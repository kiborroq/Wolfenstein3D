/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:34:01 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/17 16:34:52 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *str, int n)
{
	int		str_len;
	char	*str_cpy;

	str_len = ft_strlen(str);
	if (str_len < n)
		n = str_len;
	str_cpy = (char *)malloc((n + 1) * sizeof(char));
	if (!str_cpy)
		return (0);
	ft_memcpy(str_cpy, str, n);
	str_cpy[n] = 0;
	return (str_cpy);
}
