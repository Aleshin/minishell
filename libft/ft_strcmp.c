/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 21:01:55 by saleshin          #+#    #+#             */
/*   Updated: 2024/06/30 21:02:17 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;
	int	n;

	i = 0;
	n = ft_strlen(s2);
	if (ft_strlen(s1) > ft_strlen(s2))
		n = ft_strlen(s1);
	while ((*s1 || *s2) && i < n)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
		i++;
	}
	return (0);
}

