/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_lst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:10:09 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:10:12 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_print_sorted(t_env *lst) {
    
    char **arr;
    int len;
    if (!lst)
        return (1);
    arr = linked_list_to_envp(&lst);
    if (!arr)
        return 0;

    len = list_len(lst);

    // Bubble sort algorithm to sort arr using while loops
    int i = 0;
    while (i < len - 1) {
        int j = 0;
        while (j < len - i - 1) {
            if (ft_strcmp(arr[j], arr[j + 1]) > 0) {
                // Swap arr[j] and arr[j + 1]
                char *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
            j++;
        }
        i++;
    }

    // Print the sorted array using a while loop
    i = 0;
    while (i < len) {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putendl_fd(arr[i], STDOUT_FILENO);
        i++;
    }

    // Free allocated memory for arr
    free_arr(arr); // Assuming arr was allocated dynamically

    return 1; // Indicates success
}
