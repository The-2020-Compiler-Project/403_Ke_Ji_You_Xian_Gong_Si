#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char* tokenString)
{
	switch (token)//条件语句（else,if)整性变量声明（int）返回语句（return）以及循环语句（while）和函数类型声明（void）
	{
	case ELSE:
	case IF:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		fprintf(listing, "reserved word : %s\n", tokenString);
		break;
	case PLUS:fprintf(listing, "+\n"); break;
	case MINUS:fprintf(listing, "-\n"); break;
	case MULTIPLY:fprintf(listing, "*\n"); break;
	case DIVIDE:fprintf(listing, "/\n"); break;
	case LESSTHAN:fprintf(listing, "<\n"); break;
	case LESSOREQUAL:fprintf(listing, "<=\n"); break;
	case GREATERTHAN:fprintf(listing, ">\n"); break;
	case GREATEROREQUAL:fprintf(listing, ">=\n"); break;
	case EEQUAL:fprintf(listing, "==\n"); break;
	case UNEQUAL:fprintf(listing, "!=\n"); break;
	case EQUAL:fprintf(listing, "=\n"); break;
	case SEMI:fprintf(listing, ";\n"); break;
	case COMMA:fprintf(listing, ",\n"); break;
	case LPAREN:fprintf(listing, "(\n"); break;
	case RPAREN:fprintf(listing, ")\n"); break;
	case LMPAREN:fprintf(listing, "[\n"); break;
	case RMPAREN:fprintf(listing, "]\n"); break;
	case LLPAREN:fprintf(listing, "{\n"); break;
	case RLPAREN:fprintf(listing, "}\n"); break;
	case ENDFILE:fprintf(listing, "EOF\n"); break;
	case NUM:
		fprintf(listing, "NUM, val= %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name = %s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing, "ERROR: %s\n", tokenString);
		break;
	default:
		fprintf(listing, "Unknown token: %d\n", token);
	}
}


///语法树语句节点
TreeNode* newStmtNode(StmtKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
		t->isInFuncCom = false;
	}
	return t;
}

///语法树表达式节点
TreeNode* newExpNode(ExpKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
		t->isInFuncCom = false;
	}
	return t;
}

///语法树声明节点
TreeNode* newDeclNode(DeclKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = DeclK;
		t->kind.decl = kind;
		t->lineno = lineno;
		t->isInFuncCom = false;
	}
	return t;
}

///语法树函数参数节点
TreeNode* newParamNode(ParamKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = ParamK;
		t->kind.param = kind;
		t->lineno = lineno;
		t->isInFuncCom = false;
	}
	return t;
}

///语法树类型节点
TreeNode* newTypeNode(TypeKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = TypeK;
		t->kind.type = kind;
		t->lineno = lineno;
		t->isInFuncCom = false;
	}
	return t;
}

///复制一个字符串
char* copyString(char* s)
{
	int n;
	char* t;
	if (s == NULL)return NULL;
	n = strlen(s) + 1;
	t = (char*)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}
void printTree(TreeNode* tree)
