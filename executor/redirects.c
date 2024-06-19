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