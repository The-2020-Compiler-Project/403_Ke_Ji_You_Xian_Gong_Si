#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"


/*
DeclNode:
	FuncK: 
		attr.name = ID
		type = Void | Integer
		child[0] = TypeNode(TypeNameK) : type = Void | Integer
		child[1] = ParamNode | NULL
		child[2] = StmtNode(CompK) | NULL
		sibling = DeclNode | NULL
	VarK: 
		attr.name = ID
		type = Void | Integer
		child[0] = TypeNode(TypeNameK) : type = Void | Integer		
		sibling = DeclNode | NULL
	ArrVarK: 
		attr.arr.name = ID
		attr.arr.size = NUM
		type = Void | Integer
		child[0] = TypeNode(TypeNameK) : type = Void | Integer
		sibling = DeclNode | NULL

ParamNode:
	ArrParamK: 
		attr.arr.name = ID
		type = IntegerArray
		child[0] = TypeNode(TypeNameK) : type = IntegerArray
		sibling = ParamNode | NULL
	NonArrParamK:
		attr.name = ID
		type = Integer
		child[0] = TypeNode(TypeNameK) : type = Integer
		sibling = ParamNode | NULL

StmtNode:
	AssignK:
		child[0] = ExpNode(IdK)
		child[1] = ExpNode
		sibling = StmtNode | NULL
	CompK:
		child[0] = DeclNode( VarK | ArrVarK ) | NULL
		child[1] = StmtNode | NULL
		sibling = NULL
	IfK:
		child[0] = ExpNode(OpK)
		child[1] = StmtNode
		child[2] = StmtNode | NULL
		sibling = StmtNode | NULL
	IterK:
		child[0] = ExpNode(OpK)
		child[1] = StmtNode
		sibling = StmtNode | NULL
	RetK:
		child[0] = ExpNode | NULL

ExpNode:
	OpK:
		attr.op = + - * / < <= >= > == !=
		child[0] = ExpNode
		child[1] = ExpNode
		sibling = NULL
	ConstK:
		attr.val = NUM
		type = Integer
		child[0] = TypeNode(TypeNameK) : type = Integer
		sibling = NULL
	IdK:
		attr.name = ID
		type = Integer
		child[0] = TypeNode(TypeNameK) : type = Integer
		sibling = NULL
	ArrIdK:
		attr.arr.name = ID
		type = IntegerArray
		child[0] = TypeNode(TypeNameK) : type = IntegerArray
		child[1] = ExpNode
		sibling = NULL
	CallK:
		attr.name = ID
		type = Integer | Void
		child[0] = TypeNode(TypeNameK) : type = Integer | Void
		child[1] = ExpNode | NULL
		sibling = NULL

TypeNode:
	TypeNameK:
		type = ExpType

*/

static TokenType token; /* holds current token */

/* �ݹ���õĺ���ԭ�� */
static TreeNode* declaration_list(void); //�����б�
static TreeNode* declaration(void); //����
static TreeNode* var_declaration(void); //���ͱ�������
static TreeNode* arrvar_declaration(void); //���������������
static TreeNode* fun_declaration(void); //��������
static TreeNode* params(void); //�����ڲ���
static TreeNode* param_list(void); //�����б�
static TreeNode* param(void); //����
static TreeNode* compound_stmt(void); //�������
static TreeNode* local_declarations(void); //�ֲ���������
static TreeNode* statement_list(void); //����б�
static TreeNode* statement(void); //���
static TreeNode* assignment_stmt(void); //��ֵ���
static TreeNode* selection_stmt(void); //if���
static TreeNode* iteration_stmt(void); //while���
static TreeNode* return_stmt(void); //return���
static TreeNode* expression(void); //���ʽ
static TreeNode* additive_expression(void); //�������ʽ
static TreeNode* term(void);
static TreeNode* factor(void);
static TreeNode* args(void); //��������
static TreeNode* arg_list(void); //���������б�

//������﷨���󣬴�ӡ�д������һ�У�Ȼ����ʾ�����token
static void syntaxError(const char* message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

/* ƥ�䵱ǰtoken����ƥ�䣬��ȡ��һ��token������ƥ�䣬���������Ϣ */
static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "        ");
	}
}

/* 2. declaration_list -> declaration_list declaration | declaration */
TreeNode* declaration_list(void) /* declaration_list �� ���б� */
{
	TreeNode* t = declaration();
	TreeNode* p = t;
	while (token != ENDFILE)
	{
		TreeNode* q;
		q = declaration();
		if (q != NULL) {
			if (t == NULL)
				t = p = q;
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/* 3. declaration -> var_declaration | arrvar_declaration | fun_declaration */
TreeNode* declaration(void) /*����*/
{
	TreeNode* t = NULL;
	TreeNode* p = t;
	if (token == INT || token == VOID) {
		ExpType expType;
		if (token == INT) {
			expType = Integer;
		}
		else if (token == VOID) {
			expType = Void;
		}
		match(token);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
			if (token == SEMI) { // declaration -> var_declaration
				t = var_declaration();
				t->attr.name = idname;
				t->type = Integer;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = Integer;
				t->child[0] = q;
			}
			else if (token == LMPAREN) { // declaration -> arrvar_declaration
				t = arrvar_declaration();
				t->attr.arr.name = idname;
				t->type = IntegerArray;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = IntegerArray;
				t->child[0] = q;
			}
			else if (token == LPAREN) { // declaration -> fun_declaration
				t = fun_declaration();
				t->attr.name = idname;
				t->type = expType;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = expType;
				t->child[0] = q;
			}
			else {
				syntaxError("unexpected token -> ");
				printToken(token, tokenString);
				fprintf(listing, "        ");
			}
		}
		else {
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			token = getToken();
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/* 4. var_declaration -> type_specifier ID ; */
TreeNode* var_declaration(void) {/*���α�������*/
	TreeNode* t = newDeclNode(VarK);
	match(SEMI);
	return t;
}

/* 5. arrvar_declaration -> type_specifier ID [ NUM ] ; */
TreeNode* arrvar_declaration(void) {/*���������������*/
	TreeNode* t = newDeclNode(ArrVarK);
	match(LMPAREN);
	if (token == NUM) {
		t->attr.arr.size = atoi(tokenString);
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	match(NUM);
	match(RMPAREN);
	match(SEMI);
	return t;
}

/* 6. fun_declaration -> type_specifier ID ( params ) compound_stmt */
TreeNode* fun_declaration(void) {/*��������*/
	TreeNode* t = newDeclNode(FuncK);
	match(LPAREN);
	t->child[1] = params();
	match(RPAREN);
	t->child[2] = compound_stmt();
	t->child[2]->isInFuncCom = true;
	return t;
}



/* 8. params -> param_list | void | empty */
TreeNode* params(void) {/*�����ڲ���*/
	TreeNode* t = NULL;
	if (token == VOID) {
		match(VOID);
	}
	else if (token == INT) {
		t = param_list();
	}
	return t;
}

/* 9. param_list -> param_list , param | param */
TreeNode* param_list(void) {/*�����б�*/
	TreeNode* t = param();
	TreeNode* p = t;
	while (token == COMMA)
	{
		match(COMMA);
		TreeNode* q;
		q = param();
		if (q != NULL) {
			if (t == NULL)
				t = p = q;
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/* 10. param -> int ID | int ID [ ] */
TreeNode* param(void) {/*����*/
	TreeNode* t = NULL;
	match(INT);
	if (token == ID) {
		char* idname = copyString(tokenString);
		match(ID); //param�����һ���ַ���ID
		if (token == LMPAREN) {
			match(LMPAREN);
			match(RMPAREN); //param�����һ���ַ���']'
			t = newParamNode(ArrParamK);
			t->attr.arr.name = idname;
			t->type = IntegerArray;
			TreeNode* p = newTypeNode(TypeNameK);
			p->type = IntegerArray;
			t->child[0] = p;
		}
		else {
			t = newParamNode(NonArrParamK);
			t->attr.name = idname;
			t->type = Integer;
			TreeNode* p = newTypeNode(TypeNameK);
			p->type = Integer;
			t->child[0] = p;
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	
	return t;
}

/* 11. compound_stmt -> { local_declarations statement_list } */
TreeNode* compound_stmt(void) {/*�������*/
	TreeNode* t = newStmtNode(CompK);
	match(LLPAREN);
	t->child[0] = local_declarations(); //�������ڵ�ĵ�һ������Ϊ�ֲ������ڵ�
	t->child[1] = statement_list(); //�������ڵ�ĵڶ�������Ϊ����б�
	match(RLPAREN);
	return t;
}

/* 12. local_declarations -> local_declarations var_declaration | empty */
TreeNode* local_declarations(void) {/*�ֲ���������*/
	TreeNode* t = NULL; //����Ϊ��
	TreeNode* p = t;
	TreeNode* r = NULL;
	while (token == INT)
	{
		match(INT);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
			TreeNode* q=NULL;
			if (token == SEMI) {
				q = var_declaration();
				q->attr.name = idname;
				q->type = Integer;
				r = newTypeNode(TypeNameK);
				r->type = Integer;
				q->child[0] = r;
			}
			else if (token == LMPAREN) {
				q = arrvar_declaration();
				q->attr.arr.name = idname;
				q->type = IntegerArray;
				r = newTypeNode(TypeNameK);
				r->type = IntegerArray;
				q->child[0] = r;
			}
			else {
				syntaxError("unexpected token -> ");
				printToken(token, tokenString);
				token = getToken();
			}
			if (q != NULL) {
				if (t == NULL)
					t = p = q;
				else {
					p->sibling = q;
					p = q;
				}
			}
		}
		else {
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			token = getToken();
		}
		
	}
	return t;
}

/* 13. statement_list -> statement_list statement | empty */
TreeNode* statement_list(void) {/*����б�*/
	TreeNode* t = NULL; //����Ϊ��
	TreeNode* p = t;
	while (token == ID || token == LLPAREN || token == IF || 
		token == WHILE || token == RETURN || token == SEMI ||
		token == LPAREN || token == NUM) 
	{
		TreeNode* q;
		q = statement();
		if (q != NULL) {
			if (t == NULL)
				t = p = q;
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/* 14. statement -> assignment_stmt
				   | compound_stmt
				   | selection_stmt
				   | iteration_stmt
				   | return_stmt
*/
TreeNode* statement(void) {/*���*/
	TreeNode* t = NULL;
	if (token == ID) {
		t = assignment_stmt();
	}
	else if (token == LLPAREN) {
		t = compound_stmt();
	}
	else if (token == IF) {
		t = selection_stmt();
	}
	else if (token == WHILE) {
		t = iteration_stmt();
	}
	else if (token == RETURN) {
		t = return_stmt();
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/* 15. assignment_stmt -> ID = additive_expression ;
						| ID [ additive_expression ] = additive_expression ;
						| ID ( args ) ;
*/
TreeNode* assignment_stmt(void) {/*��ֵ���*/
	TreeNode* t = NULL;
	char* idname = copyString(tokenString);
	match(ID);
	/* assignment_stmt -> ID = additive_expression ;
						| ID [ additive_expression ] = additive_expression ; */
	if (token == EQUAL || token == LMPAREN) {
		t = newStmtNode(AssignK);
		if (t != NULL) {
			//assignment_stmt -> ID[additive_expression] = additive_expression;
			if (token == LMPAREN) {
				TreeNode* q = newExpNode(ArrIdK);
				if (q != NULL) {
					q->attr.arr.name = idname;
					q->type = Integer;
					t->child[0] = q;
					TreeNode* r = newTypeNode(TypeNameK);
					r->type = Integer;
					q->child[0] = r;
					match(LMPAREN);
					q->child[1] = additive_expression();
					match(RMPAREN);
				}

			}
			// assignment_stmt -> ID = additive_expression ;
			else {
				TreeNode* p = newExpNode(IdK);
				if (p != NULL) {
					p->attr.name = idname;
					p->type = Integer;
					t->child[0] = p;
					TreeNode* r = newTypeNode(TypeNameK);
					r->type = Integer;
					p->child[0] = r;
				}

			}
			match(EQUAL);
			t->child[1] = additive_expression();
			match(SEMI);
		}
	}
	/* assignment_stmt -> ID ( args ) ; */
	else if (token == LPAREN) {
		t = newExpNode(CallK);
		t->attr.name = idname;
		if (t != NULL) {
			match(LPAREN);
			TreeNode* r = newTypeNode(TypeNameK);
			r->type = Void;
			t->child[0] = r;
			t->child[1] = args();
			t->type = Void;
			match(RPAREN);
			match(SEMI);
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/* 16. selection_stmt -> if ( expression ) statement
					    | if ( expression ) statement else statement 
*/
TreeNode* selection_stmt(void) {/*if���*/
	TreeNode* t = newStmtNode(IfK);
	match(IF);
	match(LPAREN);
	if (t != NULL) t->child[0] = expression();
	match(RPAREN);
	if (t != NULL) t->child[1] = statement();
	if (token == ELSE) {
		match(ELSE);
		if (t != NULL) t->child[2] = statement();
	}
	return t;
}

/* 17. iteration_stmt -> while ( expression ) statement */
TreeNode* iteration_stmt(void) {/*while���*/
	TreeNode* t = newStmtNode(IterK);
	match(WHILE);
	match(LPAREN);
	if (t != NULL) t->child[0] = expression();
	match(RPAREN);
	if (t != NULL) t->child[1] = statement();
	return t;
}

/* 18. return_stmt -> return ; | return additive_expression ; */

TreeNode* return_stmt(void) {
	TreeNode* t = newStmtNode(RetK);
	if (t != NULL) {
		match(RETURN);
		if (token != SEMI) {
			t->child[0] = additive_expression();
			match(SEMI);
		}
		else {
			match(SEMI);
		}
	}
	return t;
}

/* 19. expression -> additive_expression relop additive_expression */
TreeNode* expression(void) {/*���ʽ*/
	TreeNode* t = additive_expression();
	if (token == LESSOREQUAL || token == LESSTHAN ||
		token == GREATERTHAN || token == GREATEROREQUAL ||
		token == EEQUAL || token == UNEQUAL) {
		TreeNode* p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t != NULL) {
			t->child[1] = additive_expression();
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}




/* 21. additive_expression -> additive_expression addop term | term */
TreeNode* additive_expression(void) {/*�������ʽ*/
	TreeNode* t = term();
	while (token == PLUS || token == MINUS)
	{
		TreeNode* q = newExpNode(OpK);
		if (q != NULL) {
			q->attr.op = token;
			match(token);
			q->child[0] = t;
		}
		if (q != NULL) q->child[1] = term();
		t = q;
	}
	return t;
}

/* 22. addop -> + | - */


/* 23. term -> term mulop factor | factor */
TreeNode* term(void) {
	TreeNode* t = factor();
	while (token == MULTIPLY || token == DIVIDE)
	{
		TreeNode* q = newExpNode(OpK);
		if (q != NULL) {
			q->attr.op = token;
			match(token);
			q->child[0] = t;
		}
		if (q != NULL) q->child[1] = factor();
		t = q;
	}
	return t;
}




/* 25. factor -> ( additive_expression ) | var | call | NUM */
TreeNode* factor(void) {
	TreeNode* t = NULL;
	if (token == LPAREN) { // factor -> ( additive_expression )
		match(LPAREN);
		t = additive_expression();
		match(RPAREN);
	}
	else if (token == ID) { // factor -> var | call
		char* idname = copyString(tokenString);
		match(ID);
		if (token == LMPAREN) { // factor -> ID [ additive_expression ] | ID [ ]
			match(LMPAREN);
			t = newExpNode(ArrIdK);
			t->attr.arr.name = idname;
			if (token == RMPAREN) { // factor -> ID [ ]
				t->type = IntegerArray;
				TreeNode* w = newTypeNode(TypeNameK);
				w->type = IntegerArray;
				t->child[0] = w;
			}
			else { // factor -> ID [ additive_expression ]
				t->type = Integer;
				TreeNode* w = newTypeNode(TypeNameK);
				w->type = Integer;
				t->child[0] = w;
				t->child[1] = additive_expression();
			}
			match(RMPAREN);
		}
		else if (token == LPAREN) { // factor -> call
			match(LPAREN);
			t = newExpNode(CallK);
			t->attr.name = idname;
			t->type = Integer;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = Integer;
			t->child[0] = w;
			t->child[1] = args();
			match(RPAREN);
		}
		else { // factor -> ID
			t = newExpNode(IdK);
			t->attr.name = idname;
			t->type = Integer;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = Integer;
			t->child[0] = w;
		}
			
	}
	else if (token == NUM) { // factor -> NUM
		t = newExpNode(ConstK);
		t->attr.val = atoi(copyString(tokenString));
		t->type = Integer;
		TreeNode* w = newTypeNode(TypeNameK);
		w->type = Integer;
		t->child[0] = w;
		match(NUM);
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}




/* 28. args -> arg_list | empty */
TreeNode* args(void) {/*��������*/
	TreeNode* t = NULL;
	if (token == RPAREN)
		return t;
	else {
		t = arg_list();
		return t;
	}
}

/* 29. arg_list -> arg_list , additive_expression | additive_expression */
TreeNode* arg_list(void) {/*���������б�*/
	TreeNode* t = additive_expression();
	TreeNode* p = t;
	while (token == COMMA)
	{
		TreeNode* q;
		match(COMMA);
		q = additive_expression();
		if (q != NULL) {
			if (t == NULL)
				t = p = q;
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}


/******************************************************/
/*       			   ��ӭʹ��			               */
/******************************************************/
/* 
�����﷨��
 */
TreeNode* parse(void)
{
	TreeNode* t;
	token = getToken();
	t = declaration_list(); /* 1. program -> declaration_list */
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}