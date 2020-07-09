#pragma once
/************************************************/
/*File name:code.h								*/
/*code emitting utilities for the TINY compiler	*/
/*and interface to the TM machine				*/
/************************************************/

#ifndef _CODE_H_
#define _CODE_H_

/*pc = program counter*/
#define pc 7

/*mp = "memory pointer" points
 *to the top of memory(for temp storage)*/
/*fp = "frame pointer*/
#define fp 6

/*gp = "global pointer" points
 *to bottom of memory for (global) variable storage*/
#define gp 5

/*accumulator*/
#define ac 0

/*2nd accumulator*/
#define ac1 1

#define MAX_GLOBAL_SIZE 256

/* code emitting utilities */

/*将C语言中的注释写入目标代码中*/
void emitComment(const char* c);

/*Procdure emitRO emits a register-only TM instruction
 *op=the opcode
 *r=target register
 *s=1st source register
 *t=2nd source register
 *c=acomment to be printed if TraceCode is TRUE
 */
//生成Register-only类型的目标代码
void emitRO(const char* op, int r, int s, int t,const char* c);

/*Procdure emitRM emits a register-to-memory TM instruction
 *op=the opcode 
 *r=target register
 *d=the offset
 *s=the base register
 *c=acomment to be printed if TraceCode is TRUE
 */
//生成register-to-memory类型的目标代码
void emitRM(const char* op, int r, int d, int s,const char* c);

/*Function emitSkip skips "howMany" code
 *location for later backpach.It also
 *returns the current code position
 */
//跳过指定行数的代码（为后面的程序调用使用）
int emitSkip(int howMany);

//撤销上一个函数的操作
/*Procedure emitBackup backs up to
 *loc = a previously skips location
 */
void emitBackup(int loc);

/*Procedure emitRestore restores the current
 *code position to the highest previously
 *unemitted position
 */
void emitRestore(void);

/*Procedure emitRM_Abs convert an absolute reference
 *to a pc-relative reference when emmiting a 
 *register-to-memory TM instruction
 *op=the opcode
 *r=target register
 *a=an absolute location in memory
 *c=a comment to be printed if TraceCode is TRUE
 */
/*发出寄存器到内存TM指令时，过程emmitRM_Abs将绝对引用转换
* op 操作符
* r 目标寄存器
* a 内存中的绝对位置
* c 如果TraceCode为TRUE则要打印的注释
*/
void emitRM_Abs(const char* op, int r, int a,const char* c);

#endif