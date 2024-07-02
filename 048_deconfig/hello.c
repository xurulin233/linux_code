#include <stdio.h>

void main()
{

#ifdef CONFIG_HELLO
printf("hello world!!\n");
#endif /*CONFIG_HELLO*/

printf("this is main fun \n");

}
