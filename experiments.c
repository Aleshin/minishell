#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "libft/libft.h"
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

// //декларируем структуру списка переменых среды
// typedef struct t_list 
// {
//   char *name;
//   char *value;
//   //int export; //1 or 0, when you call export it indicates that env is exported to child process
//   struct t_list *next;
// } t_env;

//функция для очистки массива строк
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

//функция делит каждую строку массива на 2 строки(до и после первого '=')
char **split_env(char *env, char c) //c == '='
{
    int len_before = 0;
    int len_after = 0;
    int i = 0;
    int size = ft_strlen(env); 
    char **res = malloc(sizeof(char *) * 2);
    if (NULL == res)
        return NULL;
    //размер строки до знака =
    while (env[len_before] != c && env[len_before] != '\0')
        len_before++;
    //this is only fornow maybe
    if (len_before == 0)
    {
        return NULL;
    }
        
    //размер строки после знака =
    len_after = size - len_before - 1;
    res[0] = malloc(sizeof(char) * len_before + 1);
    res[1] = malloc(sizeof(char) * len_after + 1);
    if (res[0] == NULL || res[1] == NULL)
    {
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
//печать списка (поменять принтф на что то другое)
void print_env(t_env *env)
{
    t_env *curr = env;

    while (curr != NULL) 
    {
        printf("%s=%s\n", curr->name, curr->value); // Print each environment variable
        curr = curr->next;
    }
}

//удаляет нодб если lst->name == name
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

int check_varname(char *str) //1 yes 0 no
{
    int res;
    
    res = 1;
    
    while(*str == ' ' || *str == '\t')
            str++;
    if (ft_isdigit(*str))
        return 0;
    while(*str != '=')
    {
        if (ft_isalpha(*str) || ft_isdigit(*str) || *str == '_')
            str++;
        else if (*str == ' ' || *str == '\t')
            return 0;
    }
    return (res);
}
//should add new var to the end of list or update existing
//экспортирует переменную в список глобальных переменных
//если просто присвоить к=555 то переменная видна только в текущем процессе
void ft_export(t_env **lst, char *str)
{
    t_env *curr = *lst;
    char **new_val = split_env(str, '=');
    //перебираем лист и если имя есть модифицируем его
    if (new_val == NULL || !check_varname(new_val[0]))
    {
        //to check if there is a name, maybe later I dont need it
        put_error_fd("export", str, "not a valid identifier");
        return ;
    }
    //maybe add here: if no '=' return


    while (curr != NULL)
    {
        if (!ft_strcmp(new_val[0], curr->name))
        {
            free(curr->value);
            curr->value = strdup(new_val[1]);
            return ;
        }
        curr = curr->next;
    }
  
    t_env *new_node = ft_lstnew_env(new_val[0], new_val[1]);
    if (new_node == NULL)
    {
        perror("Memory allocation failed for node"); 
        free_env_node(new_node);
    }
    ft_lstadd_back_env(lst, new_node);
}

//общая функция для модификации переменных среды, maybe i dont need it after
void ft_env(t_env **lst, char **argv)
{
    if ((*lst) != NULL)
	{
		if (!ft_strcmp(argv[0], "unset") && argv[1])
        {
           remove_node(lst, argv[1]);
           //printf("unsetted\n"); som env I should unset 2 times. Why?
        }
        else if (!ft_strcmp(argv[0], "env") && !argv[1])
            print_env(*lst);
        else if (!ft_strcmp(argv[0], "export")) ///what it does exactly?
        {
            ft_export(lst, argv[1]);
        }
            
        else
            put_error_fd(argv[0], argv[1], "No such file or directory");
	}
}
//generic function for exec


int main (int argc, char **argv, char **envp)
{
char *buf;
    //int id;
    (void)argc;
    (void)argv;
    //(void)envp;
    t_env *environment_list = envp_to_linked_list(envp);
    while (1) 
    {
        buf = readline("$> "); // Prompt for input command./
        if (buf == NULL || strcmp(buf, "exit") == 0) 
        {
            // If user enters exit or closes input (Ctrl+D), exit the loop
            free(buf);
            break;
        }
        // Split buf into command and arguments  init_parse_tree(buf);
        char *command = strtok(buf, " ");
        char **args = malloc(sizeof(char *) * 3); // assuming maximum 2 argument REVISAR
        args[0] = command;
        args[1] = strtok(NULL, " ");
        args[2] = NULL; // NULL terminate the array
       
        ft_env(&environment_list, args);
        free(args); // Free the memory allocated for args
        free(buf);  // Free the memory allocated for buf
    }
    //here I need to clean nvironment_list when I dont need it anymore
    lst_dealloc(&environment_list);
    return 0;
}
