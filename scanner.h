#include <iostream>
#include <string>

typedef enum{
    INT,FLOAT,CHAR,DOUBLE,VOID,MAIN,IF,ELSE,WHILE,PRINTF,SCANF,FOR,
    EQUAL,PLUS,pPLUS,MINUS,mMINUS,TIMES,DIVIDE,MODE,        ///= + - * / %
		SMALLER,SMALLEREQU,BIGGER,BIGGEREQU,NOTEQU,IFEQU,///< <= > >= != ==
    L_DA,R_DA,L_ZH,R_ZH,L_XI,R_XI,///{}[]()
		FENH,DOUH,DIAN,           ///;, .
		NUM,ID,COMMENT,RETURN,
		AUTO,BREAK,CASE,CONST,CONTINUE,
		STRUCT,MAOH,DEFAULT,SWITCH , ///后面补充的 结构体struct 以及switch语句
		HUO,ADE
}tokentype;  ///后面补充的 &&  ||

typedef struct {
  tokentype tocken_val;
  std::string str_val;
  float flo_val;  //tocken的值或为字符串、或为浮点数
}tocken_record;

class Scanner {
  public:
    int tocken_num;
    tocken_record tocken_table[1000];
    std::string a_line;
    int line_num;
    int line_end_pointerp[1000];
    void print_tocken();
    tockentype the_tocken_val(std::string str);
  private:
    int is_digit(char ch);
    int is_letter(char ch);
    int is_keyword(std::string str);
    int state_change(int state, char ch);
};
