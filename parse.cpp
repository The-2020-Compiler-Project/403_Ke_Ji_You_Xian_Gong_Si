#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"


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
