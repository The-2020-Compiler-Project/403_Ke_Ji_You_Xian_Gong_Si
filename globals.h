#pragma once
#pragma once
#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

/* MAXRESERVED 关键字的数量 */
#define MAXRESERVED 8

//TokenType tocken的种类
typedef enum {
	/*book-keeping tokens*/
	ENDFILE, ERROR,
	/*reserved words*/
	ELSE, IF, INT, RETURN, VOID, WHILE,
	/*multicaracter tokens*/
	ID, NUM,
	/*special symbols*/
	PLUS, //  + 
	MINUS, //  -
	MULTIPLY, //  * 
	DIVIDE, //  /
	LESSTHAN, //  <
	LESSOREQUAL, //  <=
	GREATERTHAN, //  >
	GREATEROREQUAL, //  >=
	EEQUAL, //  ==
	UNEQUAL, //  !=
	EQUAL, //  =
	SEMI, //  ;
	COMMA, //  ,
	LPAREN, //  (
	RPAREN, //  )
	LMPAREN, //  [
	RMPAREN, //  ]
	LLPAREN, //  {
	RLPAREN, //  }
	LNOTE, //  /*
	RNOTE  //  */
}TokenType;

//全局变量
extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;
/*****************************************************************/
/******************     语法树和语法分析     **********************/
/*****************************************************************/
/* 语句包括赋值语句、复合语句、选择语句、循环语句、返回语句
 * 表达式包括符号节点、常数节点、整型变量节点、函数调用节点
 * 声明节点包括函数声明节点、整型变量声明节点、整型数组声明节点
 * 函数参数节点包括数组参数节点、整型变量参数节点
*/
typedef enum { StmtK, ExpK,DeclK,ParamK,TypeK }NodeKind;
typedef enum {AssignK,CompK,IfK,IterK,RetK}StmtKind;
typedef enum { OpK, ConstK, IdK,ArrIdK,CallK }ExpKind;
typedef enum {FuncK,VarK, ArrVarK}DeclKind;//函数声明或变量声明
typedef enum {ArrParamK,NonArrParamK}ParamKind;
typedef enum { TypeNameK }TypeKind;

/* ArrayAttr is used for attributes for array variables */
typedef struct arrayAttr {
	TokenType type;
	char* name;
	int size;
}ArrayAttr;


typedef enum { Void, Integer, Boolean,IntegerArray }ExpType;

#define MAXCHILDREN 3

struct ScopeRec;

/*
 *语法树节点
 *包括孩子节点和兄弟节点
 * 节点分为五种类型：
 * stmt：statement语句节点
 * exp: expression表达式节点
 * decl：decline函数、参数等的声明节点
 * param：parameter 函数参数节点
 * type：type 类型节点
*/


typedef struct treeNode
{
	struct treeNode* child[MAXCHILDREN];
	struct treeNode* sibling;
	int lineno;
	NodeKind nodeKind;
	union {
		StmtKind stmt; ExpKind exp;
		DeclKind decl; ParamKind param;
		TypeKind type;
	}kind;
	union {
		TokenType op;
		TokenType type;
		int val;
		char* name;
		ArrayAttr arr;
		struct ScopeRec* scope;
	}attr;
	ExpType type;	/* 表达式中的类型检查 */
	bool isInFuncCom;  // 如果 isInFuncCom == true 入栈
	bool isAInt; // 比如a[x]是一个integer，而a不是
}TreeNode;



/*
 * EchoSource决定是否在代码每一行前加上行号
 * Transcan决定是否跟踪扫描器识别token情况
 * TracePrase TraceCode同理
*/
extern int EchoSource;
extern int TraceScan;
extern int TracePrase;
extern int TraceAnalyze;
extern int TraceCode;
extern int Error;
#endif
