#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int   a = 0;   /*全局初始化区*/    
int   b;      /*全局未初始化区 BSS*/ 
char   *p1;  
int main()    
{    
    int b;   /*栈*/     
    char  s[] = "abc";   /*栈*/
    char  *p2;   /*栈*/    
    char  *p3 = "123456";  /* 123456/0在常量区，p3在栈上*/  
    static int c = 0;   /*全局（静态）初始化区*/
    p1 = (char *)malloc(10);    
    p2 = (char *)malloc(20);    /*分配得来得10和20字节的区域就在堆*/
    strcpy( p1, "123456"); /*123456/0在常量区，编译器可能会将p1与p3指向的"123456"优化成一个地方*/
    return 0;
}


