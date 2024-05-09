#ifndef PROTO_H
# define PROTO_H

# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include "../libft/libft.h"
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

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

#endif