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

void bubble_sort(char **arr, int len)
{
    int i, j;
    char *temp;

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

int ft_print_sorted(t_env *lst) 
{    
    char **arr;
    int len;
    int i;
    
    if (!lst)
        return (1);
    arr = linked_list_to_envp(&lst);
    if (!arr)
        return 0;
    
    len = list_len(lst);
    // Bubble sort algorithm to sort arr using while loops
    bubble_sort(arr, len);
    // Print the sorted array using a while loop
    i = 0;
    while (i < len) 
    {
        if (arr[i][0] != '$')
        {
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
            ft_putendl_fd(arr[i], STDOUT_FILENO);            
        }
        i++;
    }
    // Free allocated memory for arr
    free_arr(arr); // Assuming arr was allocated dynamically
    return (1); // Indicates success
}
//function to set last exit code. it 
void set_exit_code(t_env **lst, int code)
{
   char *key;
   char *value;
   t_env *new_node;

   key = "?";
   value = ft_itoa(code); //malloc
   if (value == NULL)
   {
    perror("Malloc failed in itoa");
    return ;
   }
   new_node = NULL;
   if (!upd_envvar(key, value, *lst))
   {
        new_node = ft_lstnew_env(key, value);
        if (new_node == NULL)
        {
            perror("Memory allocation failed for node");
            free(value);
            return ;
        }
        free (value);
        ft_lstadd_back_env(lst, new_node);
    }
    else
        free(value);
}

