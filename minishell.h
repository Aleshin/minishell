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
typedef struct s_env {
  char *name;
  char *value;
  struct s_env *next;
} t_env;

#endif