#include"globals.h"
#include"util.h"
#include"scan.h"

//DFA有限状态自动机完成词法分析
/*下面为有限状态自动机的状态
  开始、数字、ID、注释开始、注释内容、完成
*/

typedef enum {
	START, INNUM, INID, INCOMMENT1, INCOMMENT2, DONE
}StateType;

char tokenString[MAXTOKENLEN + 1];
//缓存
#define BUFLEN 256
static char lineBuf[BUFLEN];//hold the current line
static int linepos = 0;//current psition in LineBuf
static int bufsize = 0;//current size of buffer string

//相当于编译原理书上的get_w()，从lineBuf中读取下一个字符
/*
如果缓存被读完了，重新读取缓存
	lineno记录源文件的代码行数
	C语言库函数从文件流读取n个字符到str中  char *fgets(char *str, int n, FILE *stream)
	linepos=0重置当前读取的缓存中的游标
	当EchoSource参数为true时
		将带有lineno代码行数的字符串缓存输出到屏幕上
		fprintf函数 C 库函数 int fprintf(FILE *stream, const char *format, ...) 发送格式化输出到流 stream 中。
否则直接返回缓存中读完的上一个字符的下一个字符
*/

static char getNextChar(void) {
	if (!(linepos < bufsize)) { 
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source)) {
			bufsize = strlen(lineBuf);
			linepos = 0;
			if (EchoSource) {
				if (lineBuf[bufsize - 1] != '\n') {
					bufsize++;
					lineBuf[bufsize - 1] = '\n';
					lineBuf[bufsize] = 0;
				}
				fprintf(listing, "%4d:%s", lineno, lineBuf);
			}
			return lineBuf[linepos++];
		}
		else return EOF;
	}
	else return lineBuf[linepos++]; //
}

/*
*将游标向前移动一位，向前读取一个字符
*/
static void ungetNextChar(void) {
	linepos--;
}

//保留字结构体
//string+token

static struct {
	char str[7];
	TokenType tok;
}reservedWords[MAXRESERVED] = { {"else",ELSE},{"if",IF},{"int",INT},{"return",RETURN},{"void",VOID},{"while",WHILE} };


//查询s字符串是否为保留字

static TokenType reservedLookUp(char* s) {
	int i;
	for (i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}
static char c = ' ', c0 = ' ';

/*
* getToken读取token串，根据扫描器的自动机进行状态的转移和输出
* save决定是否将token串保存
*/

TokenType getToken(void) {
	int tokenStringIndex = 0;
	TokenType currentToken;
	StateType state = START;
	int save;
	while (state != DONE) {
		if (c == c0)
			c0 = getNextChar();
		c = c0;
		save = TRUE;
		switch (state) {
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c) || c == '_')
				state = INID;
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;
			else {
				state = DONE;
				switch (c) {
				case EOF:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case'+':
					currentToken = PLUS;
					break;
				case'-':
					currentToken = MINUS;
					break;
				case'*':
					c0 = getNextChar();
					if (c0 == '/') {
						currentToken = RNOTE;
						c = c0;
					}
					else currentToken = MULTIPLY;
					break;
				case'/':
					c0 = getNextChar();
					if (c0 == '*') {
						currentToken = LNOTE;
						save = FALSE;
						state = INCOMMENT1;
						c = c0;
					}
					else if (c0 == '/') {
						currentToken = LNOTE;
						save = FALSE;
						state = INCOMMENT2;
						c = c0;
					}
					else currentToken = DIVIDE;
					break;
				case'<':
					c0 = getNextChar();
					if (c0 == '=') {
						currentToken = LESSOREQUAL;
						c = c0;
					}
					else currentToken = LESSTHAN;
					break;
				case'>':
					c0 = getNextChar();
					if (c0 == '=') {
						currentToken = GREATEROREQUAL;
						c = c0;
					}
					else currentToken = GREATERTHAN;
					break;
				case'=':
					c0 = getNextChar();
					if (c0 == '=') {
						currentToken = EEQUAL;
						c = c0;
					}
					else currentToken = EQUAL;
					break;
				case'!':
					c0 = getNextChar();
					if (c0 == '=') {
						currentToken = UNEQUAL;
					}
					else currentToken = ERROR;
				case';':
					currentToken = SEMI;
					break;
				case',':
					currentToken = COMMA;
					break;
				case'(':
					currentToken = LPAREN;
					break;
				case')':
					currentToken = RPAREN;
					break;
				case'[':
					currentToken = LMPAREN;
					break;
				case']':
					currentToken = RMPAREN;
					break;
				case'{':
					currentToken = LLPAREN;
					break;
				case'}':
					currentToken = RLPAREN;
					break;


				default:
					currentToken = ERROR;
					break;

				}
			}
			break;
		case INCOMMENT1:
			save = FALSE;
			c0 = getNextChar();
			if (c == '*' && c0 == '/') {
				state = START;
				c = c0;
			}
			break;
		case INCOMMENT2:
			save = FALSE;
			if (c == '\n') {
				state = START;
				c = c0;
			}
			break;
		case INNUM:
			if (!isdigit(c)) {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c) && c != '_' && !isdigit(c)) {
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
		default:
			fprintf(listing, "Scanner Bug: state=%d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;

		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = c;
		if (state == DONE) {
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
				currentToken = reservedLookUp(tokenString);
		}
	}
	if (TraceScan) {
		fprintf(listing, "\t%d:", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
}

