/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 19:18:02 by marvin            #+#    #+#             */
/*   Updated: 2024/09/13 19:18:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

int	ft_handle_heredoc(t_ast_node *current_redirect)
{
	int		pipefd[2];
	ssize_t	n;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	n = write(pipefd[WRITE_END], current_redirect->value,
			ft_strlen(current_redirect->value));
	if (n == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
	close(pipefd[WRITE_END]);
	return (pipefd[READ_END]);
}
