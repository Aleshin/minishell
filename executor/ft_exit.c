/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:00:25 by saleshin          #+#    #+#             */
/*   Updated: 2024/09/08 17:02:34 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

//returns 0 if number is 0, negative or not number
int	exit_code(char *str)
{
	long long	res;
	int			sign;
	int			is_num;

	res = 0;
	sign = 1;
	is_num = -1;
	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		is_num = 1;
		if ((sign == 1 && res > LLONG_MAX / 10)
			|| (sign == -1 && res * sign < LLONG_MIN / 10))
			return (-1);
		res = res * 10 + (*str - '0');
		str++;
	}
	if (is_num == 1 && *str == '\0')
	{
		if (sign < 0)
			res = 256 - (res % 256);
		else
			res = res % 256;
		return (res);
	}
	return (-1);
}

/*
args (num of args): command->first_child->next_sibling->next_sibling->param
arg 1: command->first_child->next_sibling->next_sibling->first_child
*/
int	ft_exit(t_ast_node *command)
{
	int		num;
	char	*value;

	num = command->first_child->next_sibling->next_sibling->param;
	if (num > 0)
	{
		value = command->first_child->next_sibling
			->next_sibling->first_child->value;
		if (exit_code(value) < 0)
		{
			printf("exit\nexit: %s: numeric argument required\n", value);
			exit (2);
		}
		if (num == 1)
			exit (exit_code(value));
		ft_putstr_fd("exit\nexit: too many arguments\n", STDERR_FILENO);
		return (130);
	}
	exit (0);
}
