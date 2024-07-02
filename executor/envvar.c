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

//печать списка (поменять принтф на что то другое)
//env
void print_env(t_env **env)
{
    t_env *curr = *env;

    while (curr != NULL)
    {
        ft_putstr_fd(curr->name, STDOUT_FILENO);
        ft_putstr_fd("=", STDOUT_FILENO);
        ft_putendl_fd(curr->value, STDOUT_FILENO);
        curr = curr->next;
    }
}

//удаляет нодб если lst->name == name
//unset
void remove_node(t_env **lst, char *name)
{
    if (*lst == NULL)
        return ;

    t_env *head = *lst;

    // Delete first node
    if (!ft_strcmp(head->name, name))
    {
        *lst = head->next; 
        free_env_node(head);
        return;
    }
    // Delete in the middle or last
    t_env *prev = head;
    t_env *curr = head->next;
    while (curr != NULL)
    {
        if (!ft_strcmp(curr->name, name))
        {
            prev->next = curr->next;
            free_env_node(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void ft_unset(t_env **list, t_ast_node *command) 
{
    t_ast_node *cur_arg;

    cur_arg = command->first_child->next_sibling->next_sibling->first_child;
    if (!cur_arg)
        return ;

    while (cur_arg != NULL) {
        remove_node(list, cur_arg->value);
        cur_arg = cur_arg->next_sibling;
    }
}

//export
void ft_export(t_env **lst, char *str)
{
    t_env *curr = *lst;
    char **new_val = split_env(str, '=');
    //перебираем лист и если имя есть модифицируем его
    if (new_val == NULL || !check_varname(new_val[0]))
    {
        //to check if there is a name, maybe later I dont need it
        printf("export %s not a valid identifier", str);
        return ;
    }
    //maybe add here: if no '=' return


    while (curr != NULL)
    {
        if (!ft_strcmp(new_val[0], curr->name))
        {
            free(curr->value);
            curr->value = ft_strdup(new_val[1]);
            return ;
        }
        curr = curr->next;
    }
  
    //add new node at the end
    t_env *new_node = ft_lstnew_env(new_val[0], new_val[1]);
    if (new_node == NULL)
    {
        perror("Memory allocation failed for node"); 
        free_env_node(new_node);
    }
    ft_lstadd_back_env(lst, new_node);
}

int list_len(t_env *env)
{
    int i = 0;
    if (env != NULL)
	{
        while (env != NULL)
		{
			i++;
            env = env->next;
		}
	}
    return i;
}