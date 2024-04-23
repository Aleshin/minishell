#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "libft/libft.h"
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
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

void print_env(t_env *env)
{
    t_env *curr = env;

    while (curr != NULL) 
    {
        printf("%s=%s\n", curr->name, curr->value); // Print each environment variable
        curr = curr->next;
    }
    //free memory //shure I have some function for this --->>>> to the separate function
    curr = env;
    t_env *temp = curr->next;
    while (curr != NULL) 
    {
        temp = curr->next;
        free_env_node(curr);
        curr = temp;
    }
}

//remove specific node where lst->name == name
void remove_node(t_env **lst, char *name)
{
    
    if (*lst == NULL)
        return ;

    t_env *head = *lst;
    //delete first node
    if (!ft_strncmp(head->name, name, ft_strlen(name)))
    {
        printf("head->name %s, user %s \n", head->name, name);
        *lst = head->next; 
        free_env_node(head);
        return;
    }
    //delete in the middle
    t_env *prev = head;
    t_env *curr = head->next;
    while(curr->next != NULL)
    {
        if (!ft_strncmp(curr->name, name, ft_strlen(name)))
        {
            prev->next = curr->next;
            free_env_node(curr);
            return; // No need to continue searching
        }
        prev = curr;
        curr = curr->next;
    }
}


//unset   export(add new var)  env
//unset ---> if var name does not exist there is no error, bash just dont do anything
void ft_env(t_env **lst, char **argv)
{
    if ((*lst) != NULL)
	{
		if (!ft_strncmp(argv[0], "unset", 5) && argv[1])  ////DOES NOT WORK, SEE HOW TO DELETE NODE WITH DATA!!!
        {
           remove_node(lst, argv[1]);
           //printf("here\n");
        }
        else if (!ft_strncmp(argv[0], "env", 3) && !argv[1])
            print_env(*lst);
        else if (!ft_strncmp(argv[0], "export", 6))
            printf("here should be export command\n");
        else
            printf("%s : No such file or directory\n", argv[0]);
	}
}

void ft_parent(int id)
{
    int status; ///int value returned by waitpid
    waitpid(id, &status, 0); // Wait for child process to finish
    if (WIFEXITED(status)) 
    {
        int statusCode = WEXITSTATUS(status);
        if (statusCode == 0)
            printf("Success!\n");
        else
            printf("Failure with status code %d:(\n", statusCode);
            //printf("Child process exited with status: %d\n", WEXITSTATUS(status));
    }
}

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
        //id = fork();
        ft_env(&environment_list, args);
        // if (id == -1) 
        // {
        //     // Fork failed
        //     perror("fork");
        //     exit(EXIT_FAILURE);
        // } 
        // else if (id == 0) 
        // {
        //     // Child process
        //     //run_child(???)
        //    ft_env(&environment_list, args);
        // } 
        // else 
        // {
        //     ft_parent(id);
        // }
        free(args); // Free the memory allocated for args
        free(buf);  // Free the memory allocated for buf
    }
    return 0;
}


// int main(int argc, char **argv, char **envp) 
// {
//     (void)argc;
//     //(void)argv;
//     // Loop through envp until you encounter a NULL pointer
//     t_env *environment_list = envp_to_linked_list(envp);
//     ft_env(environment_list, argv);
    
//     print_env(environment_list);

//     return 0;
// }

// void ft_unset(t_list env)
// {

// }

//TODO function to traverse linked list and finde node with node->name
// function to delete one node
// figure out how to add node and how to modify node->value