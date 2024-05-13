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
    Input   *input;
    Token_node   *token;
    Token_node   *token_temp;
    ASTNode *ast_root;

    (void)argc;
    (void)argv;
	(void)ast_root;
    buf = readline("$> "); // Prompt for input command./
    if (buf == NULL || strcmp(buf, "exit") == 0) {
        // If user enters exit or closes input (Ctrl+D), exit the loop
        free(buf);
        return(0);
    }
    input = (Input *)malloc(sizeof(Input));
    input->current_char = 0;
    input->current_token_type = commandLine;
    input->string = buf;
    input->token_start = 0;
    token = NULL;
    token_add(&token, input);
    token_add(&token, input);
    token_add(&token, input);
    ast_root = createASTNode(commandLine, buf);
    printf("command: %d, %s\n", token->type, token->value);
    token_temp = token;
    while (token_temp->next_token != NULL)
    {
        token_temp = token_temp->next_token;
    printf("command: %d, %s\n", token_temp->type, token_temp->value);        
    }
//    printf("command: %d, %s\n", ast_root->type, ast_root->value);
    return(0);
}
