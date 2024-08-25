/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envvar_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:05:57 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:06:28 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//varname can start 
//flag 1 for export, 0 for unset
int	check_varname(char *str, int flag) // 1 for yes, 0 for no
{
	if (*str == '\0')
		return (0);
    // Check the first character (not int nor _)
	if (!(ft_isalpha(*str) || *str == '_'))
		return (0);
	str++;
    // Check the rest of the characters
	while (*str != '\0')
	{
		if (!(ft_isalpha(*str) || ft_isdigit(*str) || *str == '_' || (flag && *str == '=')))
			return (0);
		str++;
	}
	return (1);
}

//split string to *arr[2] (before and after first '=')
char	**ft_split_global(const char *s, char c)
{
	char		**arr_of_words;
	const char	*separator;
	size_t		size_word;

    // Allocate memory for 2 elements + NULL terminator
	arr_of_words = malloc(3 * sizeof(char *));
	if (!arr_of_words)
		return (NULL);
	separator = ft_strchr(s, c);

    // If no separator is found, return NULL
	if (!separator)
	{
		free(arr_of_words);
		return (NULL);
	}
    // Allocate and copy the part before the separator
	size_word = separator - s;
	arr_of_words[0] = ft_substr(s, 0, size_word);
	if (arr_of_words[0] == NULL)
	{
		free(arr_of_words);
		return (NULL);
	}
    // Allocate and copy the part after the separator
	if (*(separator + 1) == '\0')
		arr_of_words[1] = ft_strdup("");
	else
		arr_of_words[1] = ft_strdup(separator + 1);
	if (arr_of_words[1] == NULL)
	{
		free(arr_of_words[0]);
		free(arr_of_words);
		return (NULL);
	}
	arr_of_words[2] = NULL; // NULL terminator for the array
	return (arr_of_words);
}

char	**linked_list_to_envp(t_env **env)
{
	int		i;
	int		len;
	char		**arr_of_words;
	t_env		*current;
	char		*env_string;
	
	len = list_len(*env);
	arr_of_words = malloc((len + 1) * sizeof(char *));
	i = 0;
	if (arr_of_words == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	current = *env; // Start at the head of the linked list
	while (current != NULL)
	{
        // Allocate space for the "key=value" string using ft_strjoin
		env_string = ft_strjoin(current->name, "=", current->value);
		if (env_string == NULL)
		{
			perror("malloc");
            // Free already allocated strings and array
			free_arr(arr_of_words);
			return (NULL);
		}
		arr_of_words[i++] = env_string;
		current = current->next; // Move to the next element in the linked list
	}
    // Null-terminate the array
	arr_of_words[i] = NULL;
	return (arr_of_words);
}

//функция преобразует массив строк в односвязный список
t_env	*envp_to_linked_list(char **envp)
{
	t_env	*new_node;
	t_env	*head;
	char	**arr;
	int	i;
    
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

void	free_env_node(t_env *node)
{
	if (node == NULL)
		return ;
	free(node->name);
	free(node->value);
	free(node);
}

void	lst_dealloc(t_env **head)
{
	t_env *current;
	t_env *aux;

	if (*head == NULL)
		return ;
	current = *head;
	while (current != NULL)
	{
		aux = current;
		current = current->next;
        // Free dynamically allocated memory for name and value using free_env_node
		free_env_node(aux);
	}
	*head = NULL;
}
