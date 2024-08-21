/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:10:48 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:10:55 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

int	input_redir(t_ast_node *commands)
{
	int			file;
	t_ast_node	*redirects;
	t_ast_node	*current_redirect;
	ssize_t		n;
	int			pipefd[2];

	redirects = commands->first_child;
	if (redirects == 0)
		return (-3);
	file = -3;
	current_redirect = redirects->first_child;
	while (current_redirect != NULL)
	{
		if (current_redirect->type == redirect_in)
		{
			if (file != -3)
				close(file);
			file = open(current_redirect->value, O_RDONLY);
			if (file == -1)
			{
				ft_perror("Error opening file for input redirection");
				exit(EXIT_FAILURE);
			}
		}
		else if (current_redirect->type == heredoc)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			n = write(pipefd[WRITE_END],
					current_redirect->value,
					ft_strlen(current_redirect->value));
			if (n == -1)
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			close(pipefd[WRITE_END]);
			file = pipefd[READ_END];
		}
		current_redirect = current_redirect->next_sibling;
	}
	return (file);
}

// OUTPUT REDIRECTIONS
int	output_redir(t_ast_node *commands)
{
	int			file;
	int			flags;
	t_ast_node	*redirects;
	t_ast_node	*current_redirect;

	file = -3;
	redirects = commands->first_child;
	if (redirects == NULL)
		return (-3);
	current_redirect = redirects->first_child;
	while (current_redirect != NULL)
	{
		if (current_redirect->type == redirect_out
			|| current_redirect->type == redirect_out_add)
		{
			if (file != -3)
				close(file);
			flags = O_WRONLY | O_CREAT;
			if (current_redirect->type == redirect_out_add)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			file = open(current_redirect->value, flags, 0644);
			if (file == -1)
			{
				perror("Error opening file for output redirection");
				exit(EXIT_FAILURE);
			}
		}
		current_redirect = current_redirect->next_sibling;
	}
	return (file);
}

void	handle_dup_and_close(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("dup2");
		close(old_fd);
		exit(EXIT_FAILURE);
	}
	close(old_fd);
}
