/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envvar_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:05:57 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:06:28 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_varname(char *str) // 1 for yes, 0 for no
{
    // Check if string is empty
    if (*str == '\0')
        return 0;

    // Check the first character (not int nor _)
    if (!(ft_isalpha(*str) || *str == '_'))
        return 0;
    str++;

    // Check the rest of the characters
    while (*str != '\0')
    {
        if (!(ft_isalpha(*str) || ft_isdigit(*str) || *str == '_'))
            return 0;
        str++;
    }

    return 1;
}

//split string to *arr[2] (before and after first '=')
char **split_env(char *env, char c) // c == '='
{
    int len_before;
    int len_after;
    int i;
    int size;
    char **res;
    
    len_before = 0;
    len_after = 0;
    
    size = ft_strlen(env); 
    res = malloc(sizeof(char *) * 2);
    if (res == NULL)
        return NULL;

    // string size before =
    while (env[len_before] != c && env[len_before] != '\0')
        len_before++;

    // If there is no '=', return NULL
    if (env[len_before] == '\0')
    {
        free(res);
        return NULL;
    }

    // Allocate memory for the name part
    res[0] = malloc(sizeof(char) * (len_before + 1));
    if (res[0] == NULL)
    {
        free(res);
        return NULL;
    }

    // Copy the name part
    i = 0;
    while (i < len_before)
    {
        res[0][i] = env[i];
        i++;
    }
    res[0][i] = '\0';

    // Calculate the length after '='
    len_after = size - len_before - 1;
    
    // Allocate memory for the value part
    res[1] = malloc(sizeof(char) * (len_after + 1));
    if (res[1] == NULL)
    {
        free(res[0]);
        free(res);
        return NULL;
    }

    // Copy the value part
    i = 0;
    while (i < len_after)
    {
        res[1][i] = env[len_before + 1 + i];
        i++;
    }
    res[1][i] = '\0';

    return res;
}

// Function to calculate the length of a string
size_t ft_strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}



char **linked_list_to_envp(t_env **env) {
    int i = 0;
    int len = list_len(*env);
    char **arr_of_words = malloc((len + 1) * sizeof(char *));
    if (arr_of_words == NULL) {
        perror("malloc");
        return NULL;
    }

    t_env *current = *env; // Start at the head of the linked list

    while (current != NULL) {
        // Allocate space for the "key=value" string using ft_strjoin
        char *env_string = ft_strjoin(current->name, "=", current->value);
        if (env_string == NULL) {
            perror("malloc");
            // Free already allocated strings and array
            free_arr(arr_of_words);
            return NULL;
        }

        arr_of_words[i++] = env_string;
        current = current->next; // Move to the next element in the linked list
    }
    
    // Null-terminate the array
    arr_of_words[i] = NULL;
    return arr_of_words;
}

//функция преобразует массив строк в односвязный список
t_env *envp_to_linked_list(char **envp)
{
    t_env *head = NULL;
    int i = 0;
    while (envp[i] != NULL)
    {
        //сплит каждой строки на 2
        char **arr = split_env(envp[i], '=');
        //создаем новый нод с этими строками
        t_env *new_node = ft_lstnew_env(arr[0], arr[1]);
        if (new_node == NULL)
        {
            perror("Memory allocation failed for node");
            while (head != NULL)
            {
                t_env *temp = head;
                head = head->next;
                free_env_node(temp);
            }
            return NULL;
        } 
        //добавляем этот нод в конец списка
        ft_lstadd_back_env(&head, new_node);
        i++;
        free(arr[0]);
        free(arr[1]);
        free(arr);
    }
    return head;
}

void free_env_node(t_env *node)
{
    if (node == NULL)
        return ;
    free(node->name);
    free(node->value);
    free(node);
}

void lst_dealloc(t_env **head)
{
    if (*head == NULL)
        return;

    t_env *current = *head;
    while (current != NULL)
    {
        t_env *aux = current;
        current = current->next;
        
        // Free dynamically allocated memory for name and value using free_env_node
        free_env_node(aux);
    }

    *head = NULL;
}
