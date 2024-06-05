
#include "ecos_oem_root.h"
#include "ecos_oem_database.h"
#include "ecos_configset.h"
#include "ecos_mib_functions.h"
/****************************************************************************
 *  define top struct and node.
 ****************************************************************************/

ConfigSet_t g_ConfigSet;                // system all configs saved in this.
ConfigSet_t g_ConfigSetBackup;          // backup last configs.

int ecos_oem_root_init(void)
{
    
    // 
    // init mib parser api for client.
    // 
    if (ecos_mib_parser_client_init() != 0)
    {
 
        printf("init root mib parser failed!\n");
        return -1;
    }

    // 
    // register all ecos mibs.
    // 
    ecos_mibs_system_register();            // register ecos_mib_system.c
    
    // 
    // default ecos database.
    // 
    ecos_oem_database_default(NULL);
    return 0;
}

void ecos_oem_root_free(void)
{
    // 
    // free mib parser api for client.
    // 
    ecos_mib_parser_client_free();
}