#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



int main(void)
{
	printf("@@@@@@ main start\n");
	system("./sleep.sh");
	printf("@@@@@@ main end\n");


}
