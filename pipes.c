#include "minishell.h"

typedef struct s_command {
    char *cmd;
    struct s_command *next;
} t_command;

// Function to append a command to the linked list
void append_command(t_command **head, const char *cmd) {
    t_command *new_command = (t_command *)malloc(sizeof(t_command));
    if (!new_command) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_command->cmd = ft_strdup(cmd);
    if (!new_command->cmd) {
        perror("strdup");
        free(new_command);
        exit(EXIT_FAILURE);
    }
    new_command->next = NULL;

    if (*head == NULL) {
        *head = new_command;
    } else {
        t_command *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_command;
    }
}

// Function to free the memory allocated for the linked list of commands
void free_commands(t_command *head) {
    while (head != NULL) {
        t_command *temp = head;
        head = head->next;
        free(temp->cmd);
        free(temp);
    }
}

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



int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <cmd0> <cmd1> | <cmd2> | <cmd3> | <cmd4> ...\n", argv[0]);
        return 1;
    }

    int num_pipes = 0;  // Количество пайпов
    t_command *head = NULL;

    // Проходим по аргументам командной строки
    for (int i = 1; i < argc; i++) {
        // Если встречаем вертикальную черту (|), увеличиваем счетчик пайпов
        if (strcmp(argv[i], "|") == 0) {
            num_pipes++;
            continue;
        }

        // Добавляем команду в список
        append_command(&head, argv[i]);
    }

    // Проверяем, что был хотя бы один пайп
    if (num_pipes == 0) {
        fprintf(stderr, "No pipes found in input\n");
        return 1;
    }

    // Выполняем команды через пайпы
    ft_pipes(num_pipes + 1, head);  // +1, так как количество команд = количество пайпов + 1

    // Освобождаем память, выделенную для списка команд
    free_commands(head);

    return 0;
}
