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

t_env	*ft_lstnew_env(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (new == NULL)
		return (NULL);
	new->name = strdup(name);
    new->value = strdup(value);
	new->next = NULL;
	return (new);
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
    //t_env *current = NULL;
    int i = 0;
    while (envp[i] != NULL)
    {
        char **arr = ft_split(envp[i], '=');
        t_env *new_node = ft_lstnew_env(arr[0], arr[1]);
        if (new_node == NULL)
        {
            //ft_lstclear(&head, NULL);//adjust for this struct
            t_env *temp = head;
            head = head->next;
            free(temp->name);
            free(temp->value);
            free(temp);
            perror("Memory allocation failed for node");
            return NULL;
        } 
        ft_lstadd_back_env(&head, new_node);

        i++;
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
    while (curr != NULL) {
        t_env *temp = curr;
        curr = curr->next;
        free(temp->name);
        free(temp->value);
        free(temp);
    }
    return 0;
}