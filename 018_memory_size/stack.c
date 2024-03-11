#include <stdio.h>
#include <stdlib.h>
int i = 1; /*记录申请的次数*/
void fun()
{
    char a[1024*1024]; /*一次申请 1M */
    printf("NO %d %ld 字节 地址:%p\n",i , sizeof(a), a);
    i++;
    fun();
}
int main()
{
    fun();
    return 0;
}
