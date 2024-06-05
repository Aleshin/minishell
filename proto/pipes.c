/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 21:39:48 by saleshin          #+#    #+#             */
/*   Updated: 2024/06/03 21:39:56 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void ft_pipes(int num_pipes, t_command *commands) {
    int pipe_fds[num_pipes - 1][2];
    int pids[num_pipes];

    // Создание пайпов
    int i = 0;
    while (i < num_pipes - 1) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        i++;
    }

    i = 0;
    while (i < num_pipes) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            // Дочерний процесс

            // Если это не первая команда, перенаправляем stdin на чтение из предыдущего пайпа
            if (i != 0) {
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
            }

            // Если это не последняя команда, перенаправляем stdout на запись в следующий пайп
            if (i != num_pipes - 1) {
                dup2(pipe_fds[i][1], STDOUT_FILENO);
            }

            // Закрываем все дескрипторы пайпов в дочернем процессе
            int j = 0;
            while (j < num_pipes - 1) {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
                j++;
            }

            // Токенизация команды и выполнение
            char *cmd = strtok(commands->cmd, " ");
            char *args[10];
            int k = 0;
            while (cmd != NULL) {
                args[k++] = cmd;
                cmd = strtok(NULL, " ");
            }
            args[k] = NULL;

            // Замена текущего процесса на новую команду
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }

        commands = commands->next;
        i++;
    }

    // Родительский процесс: закрываем все дескрипторы пайпов
    i = 0;
    while (i < num_pipes - 1) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
        i++;
    }

    // Ожидание завершения всех дочерних процессов
    i = 0;
    while (i < num_pipes) {
        waitpid(pids[i], NULL, 0);
        i++;
    }
}
