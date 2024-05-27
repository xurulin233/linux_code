#include "mib.h"
#include "configset.h"
#include "mib_reg.h"

 static void ParserSystemHostName(void)
{
    printf("ProductName is %s\n",g_ConfigSet.szProductName);
}


void mibs_system_register(void)
{

    mib_register
    (
        MIB_PRODUCT_NAME, "ProductName", CF_STRING, 
        REG_NODE(g_ConfigSet.szProductName), 
        ParserSystemHostName
    );

}