/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 22:31:31 by saleshin          #+#    #+#             */
/*   Updated: 2024/03/22 22:31:38 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>
#define BUF_SIZE 1024

int main (int argc, char **argv, char **envp)
{
    int i;
    char buf[BUF_SIZE];
    ssize_t nbytes;

    i = 0;
    while (envp[i] != NULL)
    {
        printf("%s\n", envp[i]);
        i++;
    }
    write(1, "$>", 2);
    nbytes = read(STDIN_FILENO, buf, BUF_SIZE - 1);
    printf("%s", buf);
    return (0);
}