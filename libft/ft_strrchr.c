/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <emikhayl@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:15:03 by emikhayl          #+#    #+#             */
/*   Updated: 2023/09/18 19:52:12 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*ends;

	ends = s;
	while (*ends)
	{
		ends++;
	}
	while (ends >= s)
	{
		if (*ends == (char)c || (*ends == '\0' && (char)c == '\0'))
		{
			return ((char *)ends);
		}
		ends--;
	}
	return (NULL);
}
/*
int	main (void)
{
	const char *str = "Hello World!";
	int char_to_find = 'l';
	char *res = ft_strrchr(str, char_to_find);
	if (res != NULL)
	{
		printf("function returns: %c on a position %ld  \n", char_to_find, res - str);
		printf("%s, %s", res, str);
	}
	else
		printf("Character %c is not found.", char_to_find);
	return (0);
}*/
