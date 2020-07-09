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

//生成Register-only类型的目标代码
void emitRO(const char* op, int r, int s, int t,const char* c);


//生成register-to-memory类型的目标代码
void emitRM(const char* op, int r, int d, int s,const char* c);

//跳过指定行数的代码（为后面的程序调用使用）
int emitSkip(int howMany);


void emitBackup(int loc);

void emitRestore(void);

/*发出寄存器到内存TM指令时，过程emmitRM_Abs将绝对引用转换
* op 操作符
* r 目标寄存器
* a 内存中的绝对位置
* c 如果TraceCode为TRUE则要打印的注释
*/
void emitRM_Abs(const char* op, int r, int a,const char* c);

#endif