#include <stdio.h>
#include "ecos_mib_comm.h"
#include "ecos_mib.h"
int main()
{
	char product_name[100];
	char product_name1[100] = "FishTank1";
	ConfigReadString(0, MIB_PRODUCT_NAME,product_name, sizeof(product_name));
	printf("init %s\n",product_name);
	ConfigWriteString(0, MIB_PRODUCT_NAME, product_name1);
	ConfigReadString(0, MIB_PRODUCT_NAME,product_name, sizeof(product_name));
	printf("write %s\n",product_name);
	return 0;
}
