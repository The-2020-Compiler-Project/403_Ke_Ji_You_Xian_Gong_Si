/************************************************/
/*File name code.cpp							*/
/*TM code emitting utilities implementation		*/
/*for the TINY compiler							*/
/************************************************/

#include "globals.h"
#include "code.h"

/*TM location number for current instruction emission*/
static int emitLoc = 0;

/*Highest TM location emitted so far 
 *For use in conjunction with emitSkip,
 *emitBackup,and emitRestore
 */
static int highEmitLoc = 0;

/*Procedure emitComment prints a comment line
 *with comment c in the code file
 */
void emitComment(const char* c) {
    if (TraceCode)fprintf(code, "*%s\n", c);
}

/*Procdure emitRO emits a register-only TM instruction
 *op=the opcode
 *r=target register
 *s=1st source register
 *t=2nd source register
 *c=acomment to be printed if TraceCode is TRUE
 */
void emitRO(const char* op, int r, int s, int t, const char* c) {
    fprintf(code, "%3d:   %5s   %d,%d,%d", emitLoc++, op, r, s, t);
    if (TraceCode)fprintf(code, "\t%s", c);
    fprintf(code, "\n");
    if (highEmitLoc < emitLoc)highEmitLoc = emitLoc;
}

/*Procdure emitRM emits a register-to-memory TM instruction
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

/*Function emitSkip skips "howMany" code
 *location for later backpach.It also
 *returns the current code position
 */
int emitSkip(int howMany) {
    int i = emitLoc;
    emitLoc += howMany;
    if (highEmitLoc < emitLoc)  highEmitLoc = emitLoc;
    return i;
}

/*Procedure emitBackup backs up to
 *loc = a previously skips location
 */
void emitBackup(int loc) {
    if (loc > highEmitLoc)emitComment("BUG in emitBackup");
    emitLoc = loc;
}

/*Procedure emitRestore restores the current
 *code position to the highest previously
 *unemitted position
 */
void emitRestore(void) {
    emitLoc = highEmitLoc;
}

/*Procedure emitRM_Abs convert an absolute reference
 *to a pc-relative reference when emmiting a
 *register-to-memory TM instruction
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