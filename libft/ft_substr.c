/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:41:30 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:26:17 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sbstr;
	size_t	s_len;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		sbstr = (char *)malloc(1 * sizeof(char));
		if (!sbstr)
			return (0);
		*sbstr = '\0';
		return (sbstr);
	}
	len = len + start > s_len ? s_len - start : len;
	sbstr = (char *)malloc((len + 1) * sizeof(char));
	if (!sbstr)
		return (0);
	ft_memcpy(sbstr, s + start, len);
	sbstr[len] = '\0';
	return (sbstr);
}
