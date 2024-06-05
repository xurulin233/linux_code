#include <stdio.h>
#include "ecos_mib_comm.h"
#include "ecos_mib.h"
int main()
{
	char product_name[100];
	ConfigReadString(0, MIB_PRODUCT_NAME,product_name, sizeof(product_name));
	printf("%s\n",product_name);
	return 0;
}
