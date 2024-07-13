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
	first_arg++; // Skip the initial '-'
	while (*first_arg)
	{
		if (*first_arg != 'n')
			return (0);
		first_arg++;
	}
	return (1);
}

int	ft_echo(t_ast_node *command)
{
	t_ast_node	*num_args;
	t_ast_node	*args;
	int			no_newline;

	num_args = command->first_child->next_sibling->next_sibling;
	args = num_args->first_child;
	no_newline = 0;
	// Check if the first argument is "-n" or any sequence of "-n"
	if (num_args != NULL && num_args->param > 0 && echo_n(args->value))
	{
		no_newline = 1;
		args = args->next_sibling;
	}
	// Print the command and its arguments
	while (args != NULL)
	{
		ft_putstr_fd(args->value, STDOUT_FILENO);
		if (args->next_sibling != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO); // Print space between arguments
		args = args->next_sibling;
	}
    // Print newline if -n flag is not set
	if (!no_newline)
		ft_putendl_fd("", STDOUT_FILENO);
	return (0); // Return success code
}

/*TESTS
echo
echo -n
echo -nnnnnnnnnnn
echo -nr -n
echo good morning
echo -n good m"orn"ing
*/
