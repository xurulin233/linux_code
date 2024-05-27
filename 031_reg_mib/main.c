#include <stdio.h>
#include "shash32.h"
#include "mib_reg.h"
#include "configset.h"
#include "mib_functions.h"

ConfigSet_t g_ConfigSet;                // system all configs saved in this.

void main_init(void)
{

    // init mib register functions.
    if (mib_reg_init() != 0)
    {
        printf("ecos_mib_reg_init error!\n");
    }

    mibs_system_register();            // register mib_system.c

}


int main()
{
    printf("start...\n");
    main_init();
    ecos_mib_show();
    printf("exit.\n");
    mib_reg_free();
    return 0;
}
