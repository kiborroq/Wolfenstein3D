/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:32:02 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:32:38 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*concat;

	if (!s1 || !s2)
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
