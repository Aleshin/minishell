#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>

//STRUCT FOR ENVIROMENT VAR
typedef struct t_list 
{
  char *name;
  char *value;
  struct t_list *next;
} t_env;

typedef struct s_struct
{
	int		i; //index
	int		n; //size?
	int		flag; //multipurpose, 0 1
	//int		start;
	int		in_fd; //file to write to
	int		out_fd; //file to read from
	char	**paths;
	//char	*path1;
	//char	*path2;
	char	**cmd; //list of commands from input 
	//char	**in_cmd; ///command reading input from a pipe
	//char	**out_cmd; //command writing output to the pipe
}	t_pipe;

//error handling
void	put_error_fd(char *bash, char *file, char *error);

#endif