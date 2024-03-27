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
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int main (int argc, char **argv, char **envp)
{
    char *buf;
    char *command;
    pid_t pid;

    (void)argc;
    (void)argv;
    (void)envp;
    while (1)
    { // Бесконечный цикл для приема команд
        buf = readline("$> "); // Запрашиваем ввод команды
        if (buf == NULL || strcmp(buf, "exit") == 0) 
        {
            // Если пользователь ввел exit или закрыл ввод (Ctrl+D), завершаем цикл
            free(buf);
            break;
        }
//    printf("%s, %s", strtok(buf, " "), strtok(NULL, " "));
    pid = fork();
    if (pid == -1)
    {
        // В случае ошибки при вызове fork()
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0)
    {
        // Код процесса-потомка
        // Заменяем образ процесса программой, которую хотим запустить
        command = strtok(buf, " ");
        execlp(command, command, strtok(NULL, " "), (char *)NULL);
        // Если execlp вернул управление, значит, произошла ошибка
        perror("execlp");
        exit(EXIT_FAILURE);
    } else
    {
        // Код процесса-родителя
        int status;
        waitpid(pid, &status, 0); // Дожидаемся завершения процесса-потомка
        if (WIFEXITED(status)) {
            printf("Программа завершилась с кодом %d\n", WEXITSTATUS(status));
        }
    }
    }
    return (0);
}