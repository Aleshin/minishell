/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_lst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:10:09 by emikhayl          #+#    #+#             */
/*   Updated: 2024/08/08 22:34:32 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bubble_sort(char **arr, int len)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

//prints array
int	ft_print_sorted(t_env *lst)
{
	char	**arr;
	int		len;
	int		i;

	i = 1;
	if (!lst)
		return (1);
	arr = linked_list_to_envp_export(&lst);
	if (!arr)
		return (0);
	len = list_len(lst);
	bubble_sort(arr, len);
	while (i < len)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(&arr[i][0], STDOUT_FILENO);
		i++;
	}
	free_arr(arr);
	return (1);
}
