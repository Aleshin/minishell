#include "minishell.h"

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

//split string to *arr[2] (before and after first '=')
char **split_env(char *env, char c) //c == '='
{
    int len_before = 0;
    int len_after = 0;
    int i = 0;
    int size = ft_strlen(env); 
    char **res = malloc(sizeof(char *) * 2);
    if (NULL == res)
        return NULL;
    //string size before =
    while (env[len_before] != c && env[len_before] != '\0')
        len_before++;
    if (len_before == 0)
    {
        return NULL;
    }
    //string size after =
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
