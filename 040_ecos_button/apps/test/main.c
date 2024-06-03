#include <stdio.h>
#include "ecos_mib_comm.h"

int main()
{
	char s_Request[100]  = "hello configserver";
	ConfigExchange((u8 *)s_Request,sizeof(s_Request));
	printf("%s\n",s_Request);
	return 0;
}
