#include"globals.h"
#include"symtab.h"
#include"analyze.h"
#include"util.h"

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
				/* not yet in table , error */
			{
				Scope nowScope = sc_top();
				symbolError(t, "undelcared symbol");
			}

			else
				/* already in table, so ignore location
				   add line number of use only */
				st_add_lineno(t->attr.arr.name, t->lineno);
			break;

		case IdK:
		case CallK:
			/* not yet in table, so treat as new definition */
			if (st_lookup(t->attr.name) == -1)
				/* not yet in table , error */
			{
				Scope nowScope= sc_top();
				symbolError(t, "undelcared symbol");
			}
				
			else
				/* already in table, so ignore location
				   add line number of use only */
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
				/* already in table, so it's an error */
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
				/*
				if (t->kind.param == NonArrParamK)
					t->type = Integer;
				else
					symbolError(t, "symbol already declared for current scope");
				*/
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

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
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
	fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
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

