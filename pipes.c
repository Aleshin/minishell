#include "minishell.h"

void	initialize_tpipe(t_pipe *data)
{
	data -> i = -1;
	data -> n = -1;
	data -> flag = 0;
	data -> in_fd = -1; //error if not set
	data -> out_fd = -1; //error if not set
	data -> paths = NULL;
	//data -> path1 = NULL;
	//data -> path2 = NULL;
	data -> in_cmd = NULL;
	data -> out_cmd = NULL;
}

void set_count_pipes(char *input, t_pipe data) 
{
    int count = 0;
    while (*input) 
    {
        if (*input == '|')
            count++;
        input++;
    }
    data.n = count; //or data->n???
}

int create_pipe(int fd[2]) {
    if (pipe(fd) < 0) {
        perror("pipe");
        return 1; // Error creating pipe
    }
    return 0; // Pipe created successfully
}
//fd[0]-->read fd[1]-->write 
void child_process(int read_fd, int write_fd) //fd 0 fd 1
{
    dup2(read_fd, STDOUT_FILENO); //redirect output to fd[1] write_fd
    close(read_fd);
    close(write_fd);
    //execute_program()
    
    int x = 0;
    if (read(read_fd, &x, sizeof(int)) < 0) {
        perror("read");
        exit(2); // Error reading from pipe
    }
    
    x += 5;
    if (write(write_fd, &x, sizeof(int)) < 0) {
        perror("write");
        exit(3); // Error writing to pipe
    }
    
    exit(0); // Child process finished successfully
}

int main(int argc, char* argv[]) {
    int fd[2];
    if (pipe(fd) == -1) {
        return;
    }
    
    int pid1 = fork();
    if (pid1 < 0) {
        return 2;
    }
    
    if (pid1 == 0) {
        // Child process 1 (ping)
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("ping", "ping", "-c", "5", "google.com", NULL);
    }
    
    int pid2 = fork();
    if (pid2 < 0) {
        return 3;
    }
    
    if (pid2 == 0) {
        // Child process 2 (grep)
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "rtt", NULL);
    }
    
    close(fd[0]);
    close(fd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    return 0;
}
