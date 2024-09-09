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

volatile sig_atomic_t	g_exit_code;

// Ctrl-C handler
void sigint_handler(int signum)
{
	(void)signum;
	// new line and prompt
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	//rl_replace_line("", 0);
	rl_redisplay();
	g_exit_code = signum;
}

// Ctrl-\ handler
void sigquit_handler(int signum)
{
	(void)signum;
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

void disable_ctrl_backslash()
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
