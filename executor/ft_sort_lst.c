#include "minishell.h"

// int compare_env(const void *a, const void *b) 
// {
//     const t_env *env_a = *(const t_env **)a;
//     const t_env *env_b = *(const t_env **)b;
//     return ft_strcmp(env_a->name, env_b->name);
// }

// // Function to print the linked list alphabetically by name
// void print_sorted_list(t_env *head) {
//     // Count the number of nodes
//     int count = list_len(head);
    
//     // Create an array of pointers to nodes
//     t_env **node_array = (t_env **)malloc(count * sizeof(t_env *));
//     if (node_array == NULL) {
//         fprintf(stderr, "Memory allocation failed\n");
//         exit(1);
//     }

//     // Populate the array with pointers to nodes
//     int index = 0;
//     current = head;
//     while (current != NULL) {
//         node_array[index++] = current;
//         current = current->next;
//     }

//     // Sort the array of pointers using qsort and the comparison function
//     qsort(node_array, count, sizeof(t_env *), compare_env);

//     // Print the sorted list
//     for (int i = 0; i < count; i++) {
//         ft_putstr_fd(node_array[i]->name, STDOUT_FILENO);
//         ft_putstr_fd("=", STDOUT_FILENO);
//         ft_putendl_fd(node_array[i]->value, STDOUT_FILENO);
//     }

//     // Free the allocated memory
//     free(node_array);
// }