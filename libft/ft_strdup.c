/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:30:38 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 11:36:49 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	size_t	str_len;
	char	*str_cpy;

	str_len = ft_strlen(str);
	str_cpy = (char *)malloc((str_len + 1) * sizeof(char));
	if (!str_cpy)
		return (0);
	return (ft_memcpy(str_cpy, str, str_len + 1));
}
