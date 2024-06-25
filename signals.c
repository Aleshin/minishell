/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 20:20:38 by saleshin          #+#    #+#             */
/*   Updated: 2024/06/25 20:20:47 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

// Ctrl-C handler
void sigint_handler(int signum)
{
	(void)signum;
	// new line and prompt
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
//	rl_replace_line("", 0);
	rl_redisplay();
}

// Ctrl-\ handler
void sigquit_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
//	rl_replace_line("", 0);
	rl_redisplay();
}

void setup_signal_handlers()
{
	struct sigaction signal;

	signal.sa_handler = sigint_handler;
	sigemptyset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART;
	sigaction(SIGINT, &signal, NULL);
	signal.sa_handler = sigquit_handler;
	sigemptyset(&signal.sa_mask);
	signal.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &signal, NULL);
}
