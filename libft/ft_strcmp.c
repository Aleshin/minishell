/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:37:31 by marvin            #+#    #+#             */
/*   Updated: 2024/09/13 22:03:50 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

//returns 0 if strings are equal, positive if 1st string is 
//greater(based in ASCII) and negativa if second is greater
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
