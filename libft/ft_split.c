/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:51:47 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 14:09:11 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_words(char const *s, char c)
{
	size_t i;
	size_t num;

	i = 0;
	num = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			num++;
		while (s[i] != c && s[i])
			i++;
	}
	return (num);
}

static char			*fill_word(char *s, char c)
{
	size_t	word_len;
	char	*word;

	word_len = 0;
	while (s[word_len] != c && s[word_len])
		word_len++;
	word = ft_substr(s, 0, word_len);
	if (!word)
		return (0);
	return (word);
}

static void			free_strs(char **strs, unsigned int n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

char				**fill_strs(char **strs, char *s, char c,
								unsigned int num_words)
{
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (j < num_words && s[i])
	{
		while (s[i] == c)
			i++;
		strs[j] = fill_word(&s[i], c);
		if (!strs[j])
		{
			free_strs(strs, j);
			return (0);
		}
		if (s[i])
			j++;
		while (s[i] != c)
			i++;
	}
	return (strs);
}

char				**ft_split(char const *s, char c)
{
	size_t	num_words;
	char	**strs;

	if (!s)
		return (0);
	num_words = count_words(s, c);
	strs = (char **)ft_calloc(num_words + 1, sizeof(char *));
	if (!strs)
		return (0);
	strs = fill_strs(strs, (char *)s, c, num_words);
	if (!strs)
		return (0);
	return (strs);
}
