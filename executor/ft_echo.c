/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emikhayl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:09:21 by emikhayl          #+#    #+#             */
/*   Updated: 2024/07/05 23:09:24 by emikhayl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//detect -n or -nnnnnnnnnnnnnnnnnnnn  0 no, 1 yes
int	echo_n(char *first_arg)
{
	if (first_arg == NULL || *first_arg != '-')
		return (0);
	first_arg++;
	while (*first_arg)
	{
		if (*first_arg != 'n')
			return (0);
		first_arg++;
	}
	return (1);
}

//return 0 ok, can not fail (to check!)
int	ft_echo(t_ast_node *command)
{
	t_ast_node	*num_args;
	t_ast_node	*args;
	int			no_newline;

	num_args = command->first_child->next_sibling->next_sibling;
	args = num_args->first_child;
	no_newline = 0;
	if (num_args != NULL && args && echo_n(args->value))
	{
		no_newline = 1;
		args = args->next_sibling;
	}
	while (args != NULL)
	{
		if (!echo_n(args->value) || no_newline == 0)
			ft_putstr_fd(args->value, STDOUT_FILENO);
		if (args->next_sibling != NULL && no_newline == 0)
			ft_putstr_fd(" ", STDOUT_FILENO);
		args = args->next_sibling;
	}
	if (!no_newline)
		ft_putendl_fd("", STDOUT_FILENO);
	return (0);
}
