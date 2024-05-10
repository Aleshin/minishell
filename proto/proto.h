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
    commandLine,
    command,
    executable,
    arguments,
    argument,
    WORD,
    SINGLE_QUOTED_STRING,
    DOUBLE_QUOTED_STRING,
    PIPE,
    WS,
    SYMBOL_UNKNOWN
} SymbolType;

typedef struct {
    SymbolType type;
    char *value;
} Token;

typedef struct ASTNode {
    SymbolType type;
    char *value; // string for symbol
    struct ASTNode *firstChild;  // Child' pointer
    struct ASTNode *nextSibling; // Sibling pointer
} ASTNode;

ASTNode* createASTNode(SymbolType type, const char *value);
void addChildNode(ASTNode *parent, ASTNode *child);
void freeAST(ASTNode *node);
#endif