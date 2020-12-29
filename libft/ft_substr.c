/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:41:30 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/04 10:05:35 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sbstr;

	if (!s)
		return (0);
	if (start + len > ft_strlen(s))
		return (ft_calloc(1, sizeof(char)));
	sbstr = (char *)malloc((len + 1) * sizeof(char));
	if (!sbstr)
		return (0);
	ft_memcpy(sbstr, s + start, len);
	sbstr[len] = '\0';
	return (sbstr);
}
