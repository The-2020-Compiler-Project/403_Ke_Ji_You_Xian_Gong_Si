#pragma once

#ifndef _UTIL_H_
#define UTIL_H_
#include"globals.h"

void printToken(TokenType, const char*);


TreeNode* newStmtNode(StmtKind);


TreeNode* newExpNode(ExpKind);


TreeNode* newDeclNode(DeclKind);


TreeNode* newParamNode(ParamKind);


TreeNode* newTypeNode(TypeKind);


char* copyString(char*);


void printTree(TreeNode*);

#endif
