#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "libft/libft.h"


typedef struct t_list 
{
  char *name;
  char *value;
  struct t_list *next;
} t_env;

void free_arr(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

//splits list in array of 2 strings(before and after '=')
char **split_env(char *env, char c) //c == '='
{
    int len_before = 0;
    int len_after = 0;
    int i = 0;
    int size = ft_strlen(env); 
    char **res = malloc(sizeof(char *) * 2);
    if (NULL == res)
        return NULL;
    //find size before
    while (env[len_before] != c && env[len_before] != '\0')
        len_before++;
    //find size after
    len_after = size - len_before - 1;
    res[0] = malloc(sizeof(char) * len_before + 1);
    res[1] = malloc(sizeof(char) * len_after + 1);
    if (res[0] == NULL || res[1] == NULL)
    {
        // free(res[1]);
        // free(res[0]);
        // free(res);
        free_arr(res);
        return NULL;
    }    
    while (i < len_before)
    {
        res[0][i] = env[i];
        i++;
    }
    res[0][i] = '\0';
    i++;
    i = 0;
    while (i < len_after)
    {
        res[1][i] = env[len_before + 1 + i];
        i++;
    }
    res[1][i] = '\0';
    return (res);
}


t_env	*ft_lstnew_env(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (NULL);
	new->name = strdup(name);
    if (new->name == NULL) 
    {
        free(new); // Free allocated t_env structure
        return (NULL);
    }
    new->value = strdup(value);
	if (new->name == NULL) 
    {
        free(new->name);
        free(new); // Free allocated t_env structure
        
        return (NULL);
    }
    new->next = NULL;
	return (new);
}

void free_env_node(t_env *lst)
{
    if (lst == NULL)
        return ;
    free(lst->name);
    free(lst->value);
    free(lst);
}

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*list;

	if (*lst == NULL)
		*lst = new;
	else
	{
		list = *lst;
		while (list->next != NULL)
		{
			list = list->next;
		}
		list->next = new;
	}
}

t_env *envp_to_linked_list(char **envp)
{
    t_env *head = NULL;
    int i = 0;
    while (envp[i] != NULL)
    {
        char **arr = split_env(envp[i], '=');
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
        ft_lstadd_back_env(&head, new_node);
        i++;
        free(arr[0]);
        free(arr[1]);
        free(arr);
    }
    return head;
}

int main(int argc, char **argv, char **envp) 
{
    (void)argc;
    (void)argv;
    // Loop through envp until you encounter a NULL pointer
    t_env *environment_list = envp_to_linked_list(envp);
    
    //print linked list
    t_env *curr = environment_list;

    while (curr != NULL) 
    {
        printf("name: %s ---> value: %s\n", curr->name, curr->value); // Print each environment variable
        curr = curr->next;
    }
    //free memory //shure I have some function for this
    curr = environment_list;
    t_env *temp = curr->next;
    while (curr != NULL) {
        temp = curr->next;
        free_env_node(curr);
        curr = temp;
    }
    return 0;
}

//TODO function to traverse linked list and finde node with node->name
// function to delete one node
// figure out how to add node and how to modify node->value