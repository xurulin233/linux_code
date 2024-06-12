#include <stdio.h>
#include "ecos_mib_comm.h"
#include "ecos_mib.h"
int main()
{
	char product_name[100];
	char product_name1[100] = "FishTank1";
	char staticip[16] = "192.168.70.185";
	char ipaddr[16];
	ConfigReadString(0, MIB_PRODUCT_NAME,product_name, sizeof(product_name));
	printf("init %s\n",product_name);
	ConfigWriteString(0, MIB_PRODUCT_NAME, product_name1);
	ConfigReadString(0, MIB_PRODUCT_NAME,product_name, sizeof(product_name));
	printf("write %s\n",product_name);
	ConfigReadString(0, MIB_NET_IP, ipaddr,sizeof(ipaddr));
	printf("read ipaddr  %s\n", ipaddr);
	ConfigWriteString(0, MIB_NET_IP, staticip);
	printf("write ipaddr  %s\n",staticip);

	return 0;
}
