#pragma once
#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"


/* SIZE is the size of the hash table */
#define SIZE 211

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
{
	int lineno;
	struct LineListRec* next;
}*LineList;

/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code 
 */
typedef struct BucketListRec
{
	char* name=NULL;
	LineList lines=NULL;
	TreeNode* treeNode=NULL;
	int memloc=0;	/* memory location for variable */
	struct BucketListRec* next=NULL;
}*BucketList;

typedef struct ScopeRec
{
	char* funcName=0;
	int nestedLevel=0;
	struct ScopeRec* parent=0;
    BucketList hashTable[SIZE] = { 0 };	/* the hash table */
}*Scope;

extern Scope globalScope;

/* Procedure st_insert inserts line numbers and 
 * memory location is inserted only the 
 * first time, otherwise ignored
 */
void st_insert(char* name, int lineno, int loc, TreeNode* treeNode);

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
int st_lookup(char* name);
int st_add_lineno(char* name, int lineno);
BucketList st_bucket(char* name);
int st_lookup_top(char* name);

Scope sc_create(char* funcName);
Scope sc_top(void);
void sc_pop(void);
void sc_push(Scope scope);
int addLocation(void);
int addLocation(int size);

/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE* listing);
#endif