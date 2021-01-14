/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:40:51 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 11:58:01 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isset(char ch, const char *set)
{
	while (*set)
	{
		if (*set == ch)
			return (1);
		set++;
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	size_t	start;
	size_t	end;
	char	*s_trim;

	if (!s1)
		return (0);
	s1_len = ft_strlen(s1);
	start = 0;
	while (isset(s1[start], set) && s1[start])
		start++;
	if (start == s1_len)
		return (ft_calloc(1, sizeof(char)));
	end = s1_len - 1;
	while (isset(s1[end], set) && end > 0)
		end--;
	s_trim = ft_substr(s1, start, end - start + 1);
	if (!s_trim)
		return (0);
	return (s_trim);
}
