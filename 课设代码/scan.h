#pragma once
#ifndef _SCAN_H_
#define _SCAN_H_
#include"globals.h"

/* MAXTOKENLEN是一个token的最大长度*/
#define MAXTOKENLEN 40

/* tokenString数组存储每个token单元 */
extern char tokenString[MAXTOKENLEN + 1];

/*  函数getToken返回源文件的下一个token
*/
TokenType getToken(void);

#endif