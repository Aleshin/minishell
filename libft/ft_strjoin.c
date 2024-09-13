/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <emikhayl@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 18:31:22 by emikhayl          #+#    #+#             */
/*   Updated: 2024/09/13 21:52:38 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
//allocates with malloc and returns new string s1 + s2 + '\0'
// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	size_t	s1_len;
// 	size_t	s2_len;
// 	char	*joined;
// 	size_t	i;

// 	s1_len = ft_strlen(s1);
// 	s2_len = ft_strlen(s2);
// 	joined = (char *)malloc(s1_len + s2_len + 1);
// 	i = 0;
// 	if (joined == NULL)
// 		return (NULL);
// 	while (*s1)
// 	{
// 		joined[i] = *s1++;
// 		i++;
// 	}
// 	while (*s2)
// 	{
// 		joined[i] = *s2++;
// 		i++;
// 	}
// 	joined[i] = '\0';
// 	return (joined);
// }
// Allocates with malloc and returns new string s1 + s2 + s3 + '\0'
//null check added to prevent segfault
char	*ft_strjoin(char const *s1, char const *s2, char const *s3)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	s3_len;
	char	*joined;
	size_t	i;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3_len = ft_strlen(s3);
	joined = (char *)malloc(s1_len + s2_len + s3_len + 1);
	i = 0;
	if (joined == NULL)
		return (NULL);
	if (s1 != NULL)
		while (*s1)
			joined[i++] = *s1++;
	if (s2 != NULL)
		while (*s2)
			joined[i++] = *s2++;
	if (s3 != NULL)
		while (*s3)
			joined[i++] = *s3++;
	joined[i] = '\0';
	return (joined);
}
/*
int main()
{
    char s1[] = "Hello ";
    char s2[] = "sunshine";
    char *join = ft_strjoin(s1, s2);
    printf("%s", join);
    free(join);
    return 0;
}*/
