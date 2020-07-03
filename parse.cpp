#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"


//如果有语法错误，打印有错误的那一行，然后提示错误的token
static void syntaxError(const char* message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

/* 匹配当前token，若匹配，获取下一个token，若不匹配，输出错误信息 */
static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "        ");
	}
}
