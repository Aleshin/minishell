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

//5
//split string to *arr[2] (before and after first '=')
//allocates memory for 2 elements and NULL terminator
char	**ft_split_global(const char *s, char c)
{
	char			**arr_of_words;
	const char		*separator;
	size_t			size_word;

	arr_of_words = malloc(3 * sizeof(char *));
	if (!arr_of_words)
		return (NULL);
	separator = ft_strchr(s, c);
	if (!separator)
		return (free(arr_of_words), NULL);
	size_word = separator - s;
	arr_of_words[0] = ft_substr(s, 0, size_word);
	if (arr_of_words[0] == NULL)
		return (free(arr_of_words), NULL);
	if (*(separator + 1) == '\0')
		arr_of_words[1] = ft_strdup("");
	else
		arr_of_words[1] = ft_strdup(separator + 1);
	if (arr_of_words[1] == NULL)
		return (free(arr_of_words[0]), free(arr_of_words), NULL);
	arr_of_words[2] = NULL;
	return (arr_of_words);
}

char	**linked_list_to_envp(t_env **env)
{
	int			i;
	int			len;
	char		**arr_of_words;
	t_env		*current;
	char		*env_string;

	len = list_len(*env);
	arr_of_words = malloc((len + 1) * sizeof(char *));
	i = 0;
	if (arr_of_words == NULL)
		return (perror("malloc"), NULL);
	current = *env;
	while (current != NULL)
	{
		if (current->to_env == 1)
			env_string = ft_strjoin(current->name, "=", current->value);
		if (env_string == NULL)
			return (free_arr(arr_of_words), perror("malloc"), NULL);
		arr_of_words[i++] = env_string;
		current = current->next;
	}
	arr_of_words[i] = NULL;
	return (arr_of_words);
}

char	**linked_list_to_envp_export(t_env **env)
{
	int			i;
	int			len;
	char		**arr_of_words;
	t_env		*current;
	char		*env_string;

	len = list_len(*env);
	arr_of_words = malloc((len + 1) * sizeof(char *));
	i = 0;
	if (arr_of_words == NULL)
		return (perror("malloc"), NULL);
	current = *env;
	while (current != NULL)
	{
		if (current->to_env == 1)
			env_string = ft_strjoin(current->name, "=", current->value);
		else if (current->to_env == 0)
			env_string = ft_strjoin(current->name, NULL, NULL);
		if (env_string == NULL)
			return (free_arr(arr_of_words), perror("malloc"), NULL);
		arr_of_words[i++] = env_string;
		current = current->next;
	}
	arr_of_words[i] = NULL;
	return (arr_of_words);
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
		new_node = ft_lstnew_env(arr[0], arr[1], 1);
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

int	list_len(t_env *env)
{
	int	i;

	i = 0;
	if (env != NULL)
	{
		while (env != NULL)
		{
			env = env->next;
			i++;
		}
	}
	return (i);
}
