#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"globals.h"
#include"symtab.h"

/* SHFIT is the power of two used as multiplier
	in hash function */
#define SHFIT 4

#define MAX_SCOPE 1000

/* the hash function */
static int hash(const char* key)
{
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{
		temp = ((temp << SHFIT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

static Scope scopes[MAX_SCOPE];
static int nScope = 0;
static Scope scopeStack[MAX_SCOPE];
static int nScopeStack = 0;
static int location[MAX_SCOPE];
Scope globalScope;

Scope sc_top(void)
{
	return scopeStack[nScopeStack - 1];
}

void sc_pop(void)
{
	--nScopeStack;
}

int addLocation(void)
{
	return location[nScopeStack - 1]++;
}

int addLocation(int size)
{
	 location[nScopeStack - 1] += size;
	 return location[nScopeStack - 1] - size;
}

void sc_push(Scope scope)
{
	scopeStack[nScopeStack] = scope;
	location[nScopeStack++] = 0;
}

Scope sc_create(char* funcName)
{
	Scope newScope;

	newScope = (Scope)malloc(sizeof(struct ScopeRec));
	newScope->funcName = funcName;
	newScope->nestedLevel = nScopeStack;
	newScope->parent = sc_top();

	for (int i = 0; i < SIZE; i++)
	{
		newScope->hashTable[i] = NULL;
	}

	scopes[nScope++] = newScope;
	return newScope;
}


// find var in stack from this stack to old stack
BucketList st_bucket(char* name)
{
	int h = hash(name);
	Scope sc = sc_top();
	while (sc) {
		BucketList l = sc->hashTable[h];
		while ((l != NULL) && (strcmp(name, l->name) != 0))
			l = l->next;
		if (l != NULL)return l;
		sc = sc->parent;
	}
	return NULL;
}

/* Procedure st_insert inserts line numbers and
 * memroy location into the symbol table
 * loc = memory location is inserted only the 
 * first time, otherwise ignored
 */
void st_insert(char* name, int lineno, int loc, TreeNode* treeNode)
{
	int h = hash(name);
	Scope top = sc_top();
	BucketList l = top->hashTable[h];
	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;
	if (l == NULL)
	{
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->next = top->hashTable[h];
		l->treeNode = treeNode;
		top->hashTable[h] = l;
	}
	else /* found in table, so just add line number */
	{
		//Error
	}
} /* st_insert */

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 * search all scopes 
 */
int st_lookup(char* name)
{
	BucketList l = st_bucket(name);
	if (l != NULL)return l->memloc;
	return -1;
}

// search top scope
int st_lookup_top(char* name)
{
	int h = hash(name);
	Scope sc = sc_top();
	while (sc) {
		BucketList l = sc->hashTable[h];
		while ((l != NULL) && (strcmp(name, l->name) != 0))
		{
			l = l->next;
		}
		if (l != NULL)return l->memloc;
		break;
	}
	return -1;
}

int st_add_lineno(char* name, int lineno)
{
	BucketList l = st_bucket(name);
	LineList ll = l->lines;
	while (ll->next != NULL)ll = ll->next;
	ll->next = (LineList)malloc(sizeof(struct LineListRec));
	ll->next->lineno = lineno;
	ll->next->next = NULL;
	return lineno;
}

void printSymTabRows(BucketList* hashTable, FILE* listing)
{
	int j;
	for (j = 0; j < SIZE; j++)
	{
		if (hashTable[j] != NULL)
		{
			BucketList l = hashTable[j];
			TreeNode* node = l->treeNode;

			while (l != NULL)
			{
				LineList t = l->lines;

				fprintf(listing, "%-14s ", l->name);

				switch (node->nodeKind) {
				case DeclK:
					switch (node->kind.decl) {
					case FuncK:
						fprintf(listing, "Function \t");
						break;
					case VarK:
						fprintf(listing, "Variable \t");
						break;
					case ArrVarK:
						fprintf(listing, "Array Var.\t");
						break;
					default:
						break;
					}
					break;
				case ParamK:
					switch (node->kind.param) {
					case NonArrParamK:
						fprintf(listing, "Variable \t");
						break;
					case ArrParamK:
						fprintf(listing, "Array Var.\t");
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}

				switch (node->type) {
				case Void:
					fprintf(listing, "Void    \t");
					break;
				case Integer:
					fprintf(listing, "Integer \t");
					break;
				case Boolean:
					fprintf(listing, "Boolean \t");
					break;
				case IntegerArray:
					fprintf(listing, "Integer \t");
				default:
					break;
				}

				while (t != NULL)
				{
					fprintf(listing, "%4d ", t->lineno);
					t = t->next;
				}

				//fprintf(listing, "memloc %d  ", l->memloc);

				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
}


/* Procedure printSymTab prints a formatted
 * listing of the symbol table contents
 * to the listing file
 */
void printSymTab(FILE* listing)
{
	int i;
	for (i = 0; i < nScope; i++)
	{
		Scope scope = scopes[i];
		BucketList* hashTable = scope->hashTable;

		if (i == 0)
		{
			fprintf(listing, "<global scope>");
		}
		else {
			fprintf(listing, "function name: %s ", scope->funcName);
		}

		fprintf(
			listing,
			"(nested level: %d)\n",
			scope->nestedLevel);

		fprintf(listing, "Symbol Name    Sym.Type         Data  Type        Line Numbers\n");
		fprintf(listing, "-----------    ---------        ----------        -------------------------------\n");

		printSymTabRows(hashTable, listing);

		fputc('\n', listing);

	}
}/* printSymTab */