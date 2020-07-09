#pragma once
#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"


/* 哈希表size */
#define SIZE 211

/* 代码所在行数列表*/
typedef struct LineListRec
{
	int lineno;
	struct LineListRec* next;
}*LineList;

/* bucket list中的每个变量的记录，包括名称、分配的内存位置和它在源代码中出现的行号列表*/
typedef struct BucketListRec
{
	char* name=NULL;
	LineList lines=NULL;
	TreeNode* treeNode=NULL;
	int memloc=0;	//memory location for variable 
	struct BucketListRec* next=NULL;
}*BucketList;

typedef struct ScopeRec
{
	char* funcName=0; //函数名
	int nestedLevel=0;  //嵌套级别
	struct ScopeRec* parent=0;
    BucketList hashTable[SIZE] = { 0 };	/* the hash table */
}*Scope;

extern Scope globalScope;

/* Procedure st_insert inserts line numbers and 
 * memory location is inserted only the 
 * first time, otherwise ignored
 */
void st_insert(char* name, int lineno, int loc, TreeNode* treeNode);//插入一个标识符

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char* name);//查找一个标识符
int st_add_lineno(char* name, int lineno);//插入标识符被使用的行号
BucketList st_bucket(char* name);//创建一个bucketlist
int st_lookup_top(char* name);//在符号表顶查找

Scope sc_create(char* funcName);//创建一个符号子表
Scope sc_top(void);//返回符号表顶
void sc_pop(void);//符号表子表出栈
void sc_push(Scope scope);//符号表子表入栈
int addLocation(void);
int addLocation(int size);

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE* listing);//符号表显示
#endif