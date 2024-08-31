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

void	ft_heredoc(const char *value, int *file)
{
	int			pipefd[2];
	ssize_t		n;

    // Create a pipe
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
    // Write the heredoc value to the write end of the pipe
	n = write(pipefd[WRITE_END], value, strlen(value));
	if (n == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
    // Close the write end of the pipe
	close(pipefd[WRITE_END]);
    // Return the read end of the pipe
	*file = pipefd[READ_END];
}

int	input_redir(t_ast_node *commands)
{
	int			file;
	t_ast_node	*redirects;
	t_ast_node	*current_redirect;

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
				ft_shell_error(current_redirect->value,
					"No such file or directory");
				exit(EXIT_FAILURE);
			}
		}
		else if (current_redirect->type == heredoc)
			ft_heredoc(current_redirect->value, &file);
		current_redirect = current_redirect->next_sibling;
	}
	return (file);
}

int open_output_file(t_ast_node *current_redirect, int current_file)
{
	int	file;
	int	flags;

    // Close the current file if it's already open
	if (current_file != -3)
		close(current_file);
    // Set flags based on redirection type
	flags = O_WRONLY | O_CREAT;
	if (current_redirect->type == redirect_out_add)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
    // Attempt to open the file
	file = open(current_redirect->value, flags, 0644);
	if (file == -1)
	{
		ft_shell_error(current_redirect->value, "No such file or directory");
		exit(EXIT_FAILURE);
	}
	return (file);
}

// OUTPUT REDIRECTIONS
int	output_redir(t_ast_node *commands)
{
	int			file;
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
			file = open_output_file(current_redirect, file);
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
