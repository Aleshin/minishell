#include "minishell.h"

//check correct var name(letters, no spaces, _, cannot start with number)
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
        else if (*str == ' ' || '\t')
            return 0;
    }
    return (res);
}

t_env    *find_last_node(t_env *head)
{
	if (NULL == head)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}


void lst_dealloc(t_env **head)
{
    if (*head == NULL)
        return ;
    t_env *current = *head;
    while (current != NULL)
    {
        t_env *aux = current;
        current = current->next;
        free(aux);
    }
    *head = NULL;
}
////
void init_new_node(t_env *new_node, int n)//here name and value
{
    new_node->name = ""; //here goes string before =
    new_node->value = ""; //here goes string after =
    new_node->next = NULL;
}

void append_node(t_env **head,  int n)
{
    t_env *new_node;
    t_env *last_node = find_last_node(*head);
    new_node = malloc(sizeof(t_env));
    if (new_node == NULL)
    {
        lst_dealloc(head);
        exit(1);
    }
    init_new_node(new_node, n);
    
    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        last_node->next = new_node;
    }
}

void init_envvar(t_env **head, char **envp)
{
    char *s;
    int i = 1;

    while(envp[i])
    {
        s = ft_atoi(argv[i]);//split env var  PWD="/Users/ekaterinamikaylova"
        append_node(head, n);
        i++;
    }
}

////look at it
//strip double and single quotes after =
char *strip_quotes(char *str)
{
    char *src = str;
    char *dest = str;
    
    // Iterate through the string
    while (*src) {
        // If the current character is not a double quote, copy it to the destination
        if (*src != '"') {
            *dest = *src;
            dest++;
        }
        src++;
    }
    
    // Null-terminate the resulting string
    *dest = '\0';
    
    return str;
}
    
//char const *s string, we should strip it from "" and if there is no quotes and whitespace after = value is empty
char	**ft_split(char const *s, char c)
{
	char	**arr_of_words;
	int		i;
	size_t	size_word;

	i = 0;
	arr_of_words = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!arr_of_words)
		return (NULL);
	while (*s)
	{
		//skip leading whitespace
 		while (*s == ' ' || *s == ' ')
 		    s++;
		if (*s == c)
			s++;
		else
		{
			size_word = 0;
			while (s[size_word] && s[size_word] != c)
				size_word++;
			arr_of_words[i++] = ft_substr(s, 0, size_word);
			if (!arr_of_words[i - 1])
				return (free_matrix(arr_of_words, i - 1));
			s = s + size_word;
		}
	}
	arr_of_words[i] = (NULL);
	return (arr_of_words);
}

int	main()
{
	char	strr[] = "Hell_f=dar'knes's.  my old. friend";
	char *str = strip_quotes(strr);
	char		c = '=';
	char		**arr_of_words;
	int			i;
	arr_of_words = ft_split(str, c);
	if (!arr_of_words)
	{
		printf("Memory allocation failed.\n");
		return 1;
	}
	i = 0;
	while (arr_of_words[i])
	{
		printf("%s\n", arr_of_words[i]);
		free(arr_of_words[i]);
		i++;
	}
	free(arr_of_words);
	return 0;
}