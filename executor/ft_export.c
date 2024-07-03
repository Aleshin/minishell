#include "minishell.h"

t_env	*ft_lstnew_env(char *name, char *value)
{
	t_env	*new;

    new = malloc(sizeof(t_env));
    if (new == NULL)
		return (NULL);
	new->name = ft_strdup(name);
    if (new->name == NULL) 
    {
        free(new); // Free allocated t_env structure
        return (NULL);
    }
    new->value = ft_strdup(value);
	if (new->name == NULL) 
    {
        free(new->name);
        free(new); // Free allocated t_env structure
        
        return (NULL);
    }
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

//export IN PROGRESS
//export without args same as env but alphabetically.
// int ft_export(t_env **lst, char *str)
// {
//     t_env *curr;
//     char **new_val; //export NEW=xxx

//     curr = *lst;
//     new_val = split_env(str, '=');
    
//     if (new_val == NULL || new_val[0] == NULL || new_val[1] == NULL || !check_varname(new_val[0]))
//     {
//         //to check if there is a name, maybe later I dont need it
//         ft_export_error(str, "not a valid identifier");
        
//         return (1);
//     }
//     //maybe add here: if no '=' return

//     //iterate list
//     while (curr != NULL)
//     {
//         if (!ft_strcmp(new_val[0], curr->name))
//         {
//             free(curr->value);
//             curr->value = ft_strdup(new_val[1]);
//             return ;
//         }
//         curr = curr->next;
//     }
  
//     //add new node at the end
//     t_env *new_node = ft_lstnew_env(new_val[0], new_val[1]);
//     if (new_node == NULL)
//     {
//         perror("Memory allocation failed for node"); 
//         free_env_node(new_node);
//     }
//     ft_lstadd_back_env(lst, new_node);
// }