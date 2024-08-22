# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>
# include <termios.h>
# include <string.h>

typedef struct t_list
{
	char		*name;
	char		*value;
	struct t_list	*next;
}	t_env;

void	lst_dealloc(t_env **head)
{
	t_env	*current;
	t_env	*aux;

	if (*head == NULL)
		return ;
	current = *head;
	while (current != NULL)
	{
		aux = current;
		current = current->next;
		free(aux->name);
	    free(aux->value);
	    free(aux);
	}
	*head = NULL;
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*substr;
	size_t	i;

	i = 0;
	s_len = strlen(s);
	if (start > s_len)
		return (strdup(""));
	else if (start + len > s_len)
		len = s_len - start;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	while (i < len)
		substr[i++] = s[start++];
	substr[i] = '\0';
	return (substr);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_split_global(const char *s, char c)
{
	char			**arr_of_words;
	const char		*separator;
	size_t			size_word;

	arr_of_words = malloc(3 * sizeof(char *));
	if (!arr_of_words)
		return (NULL);
	separator = strchr(s, c);
	if (!separator)
		return (free(arr_of_words), NULL);
	size_word = separator - s;
	arr_of_words[0] = ft_substr(s, 0, size_word);
	if (arr_of_words[0] == NULL)
		return (free(arr_of_words), NULL);
	if (*(separator + 1) == '\0')
		arr_of_words[1] = strdup("");
	else
		arr_of_words[1] = strdup(separator + 1);
	if (arr_of_words[1] == NULL)
		return (free(arr_of_words[0]), free(arr_of_words), NULL);
	arr_of_words[2] = NULL;
	return (arr_of_words);
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
		free(new);
		return (NULL);
	}
	new->value = strdup(value);
	if (new->value == NULL)
	{
		free(new->name);
		free(new);
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

t_env	*envp_to_linked_list(char **envp)
{
	t_env		*new_node;
	t_env		*head;
	char		**arr;
	int			i;

	head = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		arr = ft_split_global(envp[i], '=');
		if (arr == NULL)
			return (NULL);
		new_node = ft_lstnew_env(arr[0], arr[1]);
		if (new_node == NULL && head != NULL)
		{
			perror("Memory allocation failed for env_node");
			lst_dealloc(&head);
			return (NULL);
		}
		ft_lstadd_back_env(&head, new_node);
		free_arr(arr);
		i++;
	}
	return (head);
}

int	print_env(t_env *env)
{
	t_env	*curr;

	curr = env;
	printf("Head in env is %s\n", env->name);
	while (curr != NULL)
	{
		if (strcmp(curr->name, "?") != 0)
		{
			printf("%s", curr->name);
			printf("=");
			printf("%s\n", curr->value);
		}
		curr = curr->next;
	}
	return (0);
}

void	remove_node(t_env **lst, char *name)
{
	t_env	*tmp;
	t_env	*curr;

	if (*lst == NULL)
		return ;
	if (!strcmp((*lst)->name, name))
	{
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
		//free_env_node(tmp);
		return ;
	}
	curr = *lst;
	while (curr->next != NULL)
	{
		if (!strcmp(curr->next->name, name))
		{
			tmp = curr->next;
			curr->next = curr->next->next;
			free(tmp->name);
            free(tmp->value);
            free(tmp);
			return ;
		}
		curr = curr->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env			*list;
	(void)argc;
	(void)argv;
	
	list = envp_to_linked_list(envp);
	if (!list)
		return (1);
	print_env(list);
    remove_node(&list, "MallocNanoZone");
    printf("after deletion------------->\n");
    print_env(list);
    lst_dealloc(&list);
	return (0);
}