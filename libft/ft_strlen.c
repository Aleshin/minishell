/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <emikhayl@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 21:57:42 by emikhayl          #+#    #+#             */
/*   Updated: 2024/09/13 21:54:16 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	if (str == NULL)
		return (0);
	while (*str)
	{
		str++;
		len++;
	}
	return (len);
}
/*
int	main (void)
{
	const char *str = "Hello";
	size_t length = ft_strlen(str);
// z is size_t and u is unsigned
	printf("Length of the string is: %zu\n", length);
	return (0);
}*/
