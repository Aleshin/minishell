/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:39:57 by saleshin          #+#    #+#             */
/*   Updated: 2024/05/10 19:40:01 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "proto.h"

ASTNode* createASTNode(SymbolType type, const char *value)
{
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = strdup(value);  // Дублирование строки для безопасного хранения
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

void addChildNode(ASTNode *parent, ASTNode *child)
{
    if (parent->firstChild == NULL) {
        parent->firstChild = child;  // Если у родителя нет детей, добавить первого ребенка
    } else {
        ASTNode *current = parent->firstChild;
        while (current->nextSibling != NULL) {
            current = current->nextSibling;  // Перемещение к последнему ребенку
        }
        current->nextSibling = child;  // Добавление нового ребенка в конец списка братьев
    }
}

void freeAST(ASTNode *node)
{
    if (node->value) {
        free(node->value);
    }
    ASTNode *current = node->firstChild;
    while (current != NULL) {
        ASTNode *next = current->nextSibling;
        freeAST(current);
        current = next;
    }
    free(node);
}
