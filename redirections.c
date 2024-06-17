
#include <fcntl.h>
#include "minishell.h"

void free_arr(char **arr)
{
    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void ft_perror(char *str)
{
	perror(str);
    exit(EXIT_FAILURE);
}

static void	print_error(char *command) 
{
    int cmd_len = strlen(command);
  
    // Write "command not found: "
    if (write(2, "command not found: ", 19) < 0) 
        ft_perror("write");
    // Write the command
    if (write(2, command, cmd_len) < 0)
        ft_perror("write");
    // Write newline character
    if (write(2, "\n", 1) < 0)
        ft_perror("write");
}

char *ft_find_abs_path(char *command) 
{
    char *path = getenv("PATH");
    char **arr = ft_split(path, ':');
    if (!arr)
        return NULL;

    char *path_to_command = NULL;
    int i = 0;
    while (arr[i]) {
        char *tmp = ft_strjoin(arr[i], "/", command);
        if (access(tmp, F_OK | X_OK) != -1) 
        {
            path_to_command = tmp;
            break;
        }
        free(tmp);
        i++;
    }
    free_arr(arr);
    return path_to_command;
}


int redir_type(char *str)
{
    if (!str || ft_strlen(str) > 2) 
        return 0;

    while (*str)
    {
        if (*str == '>' && *(str + 1) == '\0') // redirect output
            return 1;
        else if (*str == '>' && *(str + 1) == '>') // redirect output append mode
            return 2;
        else if (*str == '<' && *(str + 1) == '\0') // redirect input
            return 3;
        else if (*str == '<' && *(str + 1) == '<') // read input till delimiter
            return 4;
        str++;
    }
    return 0;
}

//function that checks path and if it exists execute execve
void ft_execute(char	**commands)
{
	char *path;
	//char **argv;
	
	path = ft_find_abs_path(commands[0]);
    if (path == NULL) 
	{
		print_error(commands[0]);
        //fprintf(stderr, "Command not found: %s\n", commands->first_child->value);
        exit(EXIT_FAILURE);
    }
    //execve(path, argv, NULL);
    if (execve(path, commands, NULL) == -1) ///NULL stands for inherit env from the calling process, e.g. minishell
    { 
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

void ft_redirect(char **cmd, int redir, char *filename)
{
    int pid;
    int file;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (redir == 1) //>
        {
            file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);        
            if (file == -1)
            {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }
            // Redirect stdout to the opened file
            if (dup2(file, STDOUT_FILENO) < 0)
                ft_perror("dup2 error");
            close(file);
        }
        else if (redir == 2) // >>
        {
            file = open(filename, O_WRONLY| O_CREAT | O_APPEND, 0644);        
            if (file == -1)
            {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }
            // Redirect stdout to the opened file
            if (dup2(file, STDOUT_FILENO) < 0)
                ft_perror("dup2 error");
            close(file);
        }
        else if (redir == 3) // <
        {
            file = open(filename, O_RDONLY);
            if (file == -1) {
                perror("Error opening file. File dont exist");
                exit(EXIT_FAILURE);
            }
            // Redirect stdin from the opened file
            if (dup2(file, STDIN_FILENO) < 0)
                ft_perror("dup2 error");
            close(file);
        }
        // Execute the command
        if (cmd[0] != NULL)
            ft_execute(cmd);
    } else {
        // Parent process
        int wstatus;
        waitpid(pid, &wstatus, 0);

        if (WIFEXITED(wstatus)) {
            int statusCode = WEXITSTATUS(wstatus);
            if (statusCode == 0) {
                printf("Success\n");
            } else {
                printf("Failure with status code %d\n", statusCode);
            }
        } else if (WIFSIGNALED(wstatus)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(wstatus));
        }
    }
}

int main() {
    char *cmd[] = {"cat", NULL}; // Array of strings
    char *operator = ">";
    char *file_name = "outfile.txt";
    int oper = redir_type(operator);
    ft_redirect(&cmd[0], oper, file_name);
    
    return 0;
}