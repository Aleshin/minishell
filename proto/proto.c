/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 20:49:38 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/09 20:50:28 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "proto.h"

int main(int argc, char** argv)
{
    char *buf;
    ASTNode *ast_root;

    (void)argc;
    (void)argv;
    buf = readline("$> "); // Prompt for input command./
    if (buf == NULL || strcmp(buf, "exit") == 0) {
        // If user enters exit or closes input (Ctrl+D), exit the loop
        free(buf);
        return(0);
    }
    ast_root = createASTNode(commandLine, buf);
    printf("command: %d, %s\n", ast_root->type, ast_root->value);
    return(0);
}
