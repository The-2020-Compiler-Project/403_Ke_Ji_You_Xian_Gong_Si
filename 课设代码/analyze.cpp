#include"globals.h"
#include"symtab.h"
#include"analyze.h"
#include"util.h"

static char* funcName;
static int preserveLastScope = FALSE;


 /* 变量内存地址计数器*/
static int location = 0;

/* traverse函数是一个对递归语法树
 * 的遍历
 */
static void traverse(TreeNode* t,
	void (*preProc)(TreeNode*),
	void (*postProc)(TreeNode*))
{
	if (t != NULL)
	{
		preProc(t);
		{
			int i;
			for (i = 0; i < MAXCHILDREN; i++)
				traverse(t->child[i], preProc, postProc);

		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

static void insertIOFunc(void)
{
	TreeNode* func;
	TreeNode* typeSpec;
	TreeNode* param;
	TreeNode* compStmt;

	func = newDeclNode(FuncK);

	typeSpec = newTypeNode(TypeNameK);
	typeSpec->attr.type = INT;
	func->type = Integer;

	compStmt = newStmtNode(CompK);
	compStmt->child[0] = NULL;	// no local var
	compStmt->child[1] = NULL;	// no stmt

	func->lineno = 0;
	char* funcName = new char[10];

	strcpy(funcName, "input");
	func->attr.name = funcName;
	func->child[0] = typeSpec;
	func->child[1] = NULL;	// no param
	func->child[2] = compStmt;

	char* tempName = new char[10];
	strcpy(tempName, "input");
	
	st_insert(tempName, -1, addLocation(), func);

	func = newDeclNode(FuncK);

	typeSpec = newTypeNode(TypeNameK);
	typeSpec->attr.type = VOID;
	func->type = Void;

	param = newParamNode(NonArrParamK);
	char* tempName2 = new char[10];
	strcpy(tempName2, "arg");
	func->attr.name = tempName2;
	param->child[0] = newTypeNode(TypeNameK);
	param->child[0]->attr.type = INT;

	compStmt = newStmtNode(CompK);
	compStmt->child[0] = NULL;
	compStmt->child[1] = NULL;

	func->lineno = 0;
	char* tempName3 = new char[10];
	strcpy(tempName3, "output");
	func->attr.name = tempName3;
	func->child[0] = typeSpec;
	func->child[1] = param;
	func->child[2] = compStmt;
	
	char* tempName1 = new char[10];
	strcpy(tempName1, "output");
	st_insert(tempName1, -1, addLocation(), func);
}

static void symbolError(TreeNode* t,const char* message)
{
	fprintf(listing, "在%d行出现符号错误: %s\n", t->lineno, message);
	Error = TRUE;
}


static void nullProc(TreeNode* t)
{
	if (t == NULL)return;
	else return;
}


static void insertNode(TreeNode* t)
{
	switch (t->nodeKind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case CompK:
			if (preserveLastScope) {
				preserveLastScope = FALSE;
			}
			else {
				if (t->isInFuncCom)
				{
					Scope scope = sc_create(funcName);
					sc_push(scope);
				}
				
			}
			t->attr.scope = sc_top();
			break;
		default:
			break;
		
		}
		break;
	case ExpK:
		switch (t->kind.exp)
		{
		
		case ArrIdK:
			if (st_lookup(t->attr.arr.name) == -1)
			{
				Scope nowScope = sc_top();
				symbolError(t, "undelcared symbol");
			}

			else
				st_add_lineno(t->attr.arr.name, t->lineno);
			break;

		case IdK:
		case CallK:
			if (st_lookup(t->attr.name) == -1)
			{
				Scope nowScope= sc_top();
				symbolError(t, "undelcared symbol");
			}
				
			else
				st_add_lineno(t->attr.name, t->lineno);
			break;
		default:
			break;
		}
		break;
	case DeclK:
		switch (t->kind.decl)
		{
		case FuncK:
			funcName = t->attr.name;
			if (st_lookup_top(funcName) >= 0) {
				symbolError(t, "function already declared");
				break;
			}
			

			st_insert(funcName, t->lineno, addLocation(), t);
			sc_push(sc_create(funcName));
			preserveLastScope = TRUE;
			switch (t->child[0]->type)
			{
			case Integer:
				t->type = Integer;
				break;
			case Void:
			default:
				t->type = Void;
				break;
			}
			break;
			case VarK:
			case ArrVarK:
			{
				char* name;

				if (t->child[0]->attr.type == VOID) {
					symbolError(t, "variable should have non-void type");
					break;
				}
				if (t->kind.decl == VarK) {
					name = t->attr.name;
				}
				else {
					name = t->attr.arr.name;

				}

				if (st_lookup_top(name) < 0)
				{
					
					if (t->type == Integer)
					{
						st_insert(name, t->lineno, addLocation(), t);
					}
					else if (t->type == IntegerArray)
					{
						st_insert(name, t->lineno, addLocation(t->attr.arr.size), t);
					}
					
				}
				else
					symbolError(t, "symbol already declared for current scope");
			}
			break;
			default:
				break;

		}
		break;
	case ParamK:
		if(t->child[0]->attr.type==VOID)
			symbolError(t->child[0], "void type parameter is not allowed");
		else if (t->kind.param == NonArrParamK)
		{
			if (st_lookup(t->attr.name) == -1) {
				st_insert(t->attr.name, t->lineno, addLocation(), t);
				if (t->kind.param == NonArrParamK)
					t->type = Integer;
				else
					symbolError(t, "symbol already declared for current scope");
			}
		}
		else if (t->kind.param == ArrParamK)
		{
			if (st_lookup(t->attr.arr.name) == -1) {
				st_insert(t->attr.arr.name, t->lineno, addLocation(), t);
			}
		}
		
		break;
	default:
		break;
	}

}

static void afterInsertNode(TreeNode* t)
{
	switch (t->nodeKind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case CompK:
			if (t->isInFuncCom)
			{
				sc_pop();
			}
			
			break;
		default:
			break;
		}
	}
}

void buildSymtab(TreeNode* syntaxTree)
{
	globalScope = sc_create(NULL);
	sc_push(globalScope);
	insertIOFunc();
	traverse(syntaxTree, insertNode, afterInsertNode);
	sc_pop();
	if (TraceAnalyze)
	{
		fprintf(listing, "\nSymbol table:\n\n");
		printSymTab(listing);
	}
}

static void typeError(TreeNode* t, const char* message)
{
	fprintf(listing, "在%d行出现类型错误: %s\n", t->lineno, message);
	Error = TRUE;
}

static void beforeCheck(TreeNode* t)
{
	switch (t->nodeKind)
	{
	case DeclK:
		switch (t->kind.decl)
		{
		case FuncK:
			funcName = t->attr.name;
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt)
		{
		case CompK:
			sc_push(t->attr.scope);
			break;
		default:
			break;
		}
	default:
		break;
	}
}



/* checkNode函数检查单个语法树节点的类型 */
static void checkNode(TreeNode* t)
{
	switch (t->nodeKind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case AssignK:
			if (t->child[0]->child[0]->type == IntegerArray)
				typeError(t->child[0], "assignment to array variable");
			else if (t->child[1]->type == Void)
				typeError(t->child[0], "assignment of void value");
			else
				t->type = t->child[0]->type;
			break;
		case CompK:
			sc_pop();
			break;
		case IterK:
			if (t->child[0]->type == Void)
				/* while test should be void function call */
				typeError(t->child[0], "while test has void value");
			break;
		case RetK:
		{
			const TreeNode* funcDecl =
				st_bucket(funcName)->treeNode;
			const ExpType funcType = funcDecl->type;
			const TreeNode* expr = t->child[0];

			if (funcType == Void &&
				(expr != NULL && expr->type != Void)) {
				typeError(t, "expected no return value");
				//ValueReturned = TRUE;
			}
			else if (funcType == Integer &&
				(expr == NULL || expr->type == Void)) {
				typeError(t, "expected return vaule");
			}
		}
		break;
		default:
			break;
		}
		break;
	case ExpK:
		switch (t->kind.exp)
		{
		
		case OpK:
		{
			ExpType leftType, rightType;
			TokenType op;

			leftType = t->child[0]->type;
			rightType = t->child[1]->type;
			op = t->attr.op;

			if (leftType == Void ||
				rightType == Void)
				typeError(t, "two operands should have non-void type");
			else if (leftType == IntegerArray &&
				rightType == IntegerArray)
				typeError(t, "not both of operands can be array");
			else if (op == MINUS &&
				leftType == Integer &&
				rightType == IntegerArray)
				typeError(t, "invalid operands to binary expression");
			else if ((op == MULTIPLY || op == DIVIDE) &&
				(leftType == IntegerArray ||
					rightType == IntegerArray))
				typeError(t, "invalid operands to binary expression");
			else {
				t->type = Integer;
			}
		}
		break;
		case ConstK:
			t->type = Integer;
			break;
		case IdK:
		case ArrIdK:
		{
			char* symbolName;
			if (t->kind.exp == IdK)
			{
				symbolName = t->attr.name;
			}
			else
			{
				symbolName = t->attr.arr.name;
			}
			const BucketList bucket =
				st_bucket(symbolName);
			TreeNode* symbolDecl = NULL;

			if (bucket == NULL)
				break;
			symbolDecl = bucket->treeNode;

			if (t->type == IntegerArray) {
				if (symbolDecl->type != IntegerArray)
					typeError(t, "expected array symbol");
				else if (t->child[1]!=NULL&& t->child[1]->type != Integer)
					typeError(t, "index expression should have integer type");
				/*
				else
					t->type = Integer;
				*/
			}
			/*
			else {
				t->type = symbolDecl->type; ////////////////////////////////////////////////////////////maybe change int to arrint
			}
			*/

		}
		break;
		case CallK:
		{
			char* callingFuncName = t->attr.name;

			BucketList funcBucketList = st_bucket(callingFuncName);
			if (funcBucketList == NULL)
				break;
			TreeNode* funcDecl =
				funcBucketList->treeNode;
			TreeNode* arg;
			TreeNode* param;

			if (funcDecl == NULL)
				break;

			arg = t->child[1];
			param = funcDecl->child[1];

			if (funcDecl->kind.decl != FuncK)
			{
				typeError(t, "expected function symbol");
				break;
			}

			while (arg != NULL)
			{
				if (param == NULL)
					typeError(arg, "the number of parameters is wrong");
				else if (arg->type == Void)
					typeError(arg, "void value cannot be passed as an argument"); \
				else { // no problem! all right!
					arg = arg->sibling;
					param = param->sibling;
					continue;
				}
				/* any problem */
				break;

			}
			if (arg == NULL && param != NULL)
				typeError(t->child[0], "the number of parameters is wrong");

		}
		break;
		default:
			break;
		}
		break;
	default:
		break;
		break;
	}
}

/* typeCheck函数
 * 通过后序语法树遍历完成类型检查
 */
void typeCheck(TreeNode* syntaxTree)
{
	sc_push(globalScope);
	traverse(syntaxTree, beforeCheck, checkNode);
	sc_pop();
}