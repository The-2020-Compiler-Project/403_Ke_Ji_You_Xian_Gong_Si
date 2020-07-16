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

static int indentno = 0;

#define INDENT indentno+=2;
#define UNINDENT indentno -= 2;

static void printSpaces(void)
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

void printTree(TreeNode* tree)
{
	int i;
	INDENT;
	while (tree != NULL) {
		printSpaces();
		if (tree->nodeKind == StmtK)
		{
			switch (tree->kind.stmt) {
			case IfK:
				fprintf(listing, "If\n");
				break;
			case IterK:
				fprintf(listing, "While\n");
				break;
			case AssignK:
				fprintf(listing, "Assign\n");
				break;
			case CompK:
				fprintf(listing, "Compound Statement\n");
				break;
			case RetK:
				fprintf(listing, "Return\n");
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == ExpK)
		{
			switch (tree->kind.exp) {
			case OpK:
				fprintf(listing, "Op: ");
				printToken(tree->attr.op, "\0");
				break;
			case ConstK:
				fprintf(listing, "Const: %d\n", tree->attr.val);
				break;
			case IdK:
				fprintf(listing, "Id: %s\n", tree->attr.name);
				break;
			case ArrIdK:
				fprintf(listing, "ArrId: %s\n", tree->attr.arr.name);
				break;
			case CallK:
				fprintf(listing, "Call(followings are args): %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == DeclK) {
			switch (tree->kind.decl) {
			case VarK:
				fprintf(listing, "Var Decl: %s\n", tree->attr.name);
				break;
			case ArrVarK:
				fprintf(listing, "Var Decl(following const:array length): %s %d\n",
					tree->attr.arr.name, tree->attr.arr.size);
				break;
			case FuncK:
				fprintf(listing, "Funcion Decl: %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unknown DeclNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == ParamK) {
			switch (tree->kind.param) {
			case ArrParamK:
				fprintf(listing, "Array Parameter: %s\n", tree->attr.arr.name);
				break;
			case NonArrParamK:
				fprintf(listing, "Parameter: %s\n", tree->attr.name);
				break;
			default:
				fprintf(listing, "Unknown ParamNode kind\n");
				break;
			}
		}
		else if (tree->nodeKind == TypeK) {
			switch (tree->type) {
			case Void:
				fprintf(listing, "Type: void\n");
				break;
			case Integer:
				fprintf(listing, "Type: int\n");
				break;
			case IntegerArray:
				fprintf(listing, "Type: int array\n");
				break;
			default:
				fprintf(listing, "Unknown TypeNode kind\n");
				break;
			}
		}
		else fprintf(listing, "Unknown node kind\n");
		for (i = 0; i < MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}
