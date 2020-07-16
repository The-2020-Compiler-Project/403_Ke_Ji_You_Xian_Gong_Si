#include "globals.h"
#include "code.h"

static int emitLoc = 0;

static int highEmitLoc = 0;

/*Procedure emitComment prints a comment line
 *with comment c in the code file
 */
void emitComment(const char* c) {
    if (TraceCode)fprintf(code, "*%s\n", c);
}

/*emitRO函数释放一个只对寄存器操作的指令
 *op ：the opcode
 *r ： target register
 *s ：1st source register
 *t ：2nd source register
 *c ：a comment to be printed if TraceCode is TRUE
 */
void emitRO(const char* op, int r, int s, int t, const char* c) {
    fprintf(code, "%3d:   %5s   %d,%d,%d", emitLoc++, op, r, s, t);
    if (TraceCode)fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc)highEmitLoc = emitLoc;
}

/*emitRM函数释放一个寄存器--内存操作的指令
 *op=the opcode
 *r=target register
 *d=the offset
 *s=the base register
 *c=acomment to be printed if TraceCode is TRUE
 */
void emitRM(const char* op, int r, int d, int s,const char* c) {
    fprintf(code, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op, r, d, s);
    if (TraceCode) fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
}


int emitSkip(int howMany) {
    int i = emitLoc;
    emitLoc += howMany;
    if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
    return i;
}

void emitBackup(int loc) {
    if (loc > highEmitLoc)emitComment("BUG in emitBackup");
    emitLoc = loc;
}

void emitRestore(void) {
    emitLoc = highEmitLoc;
}

/*函数emitRM_Abs将寄存器-内存操作指令中的地址进行转换
 *op=the opcode
 *r=target register
 *a=an absolute location in memory
 *c=a comment to be printed if TraceCode is TRUE
 */
void emitRM_Abs(const char* op, int r, int a,const char* c) {
    fprintf(code, "%3d:   %5s %d,%d(%d)", emitLoc, op, r, a - (emitLoc + 1),pc);
    ++emitLoc;
    if (TraceCode)fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc)highEmitLoc = emitLoc;
}