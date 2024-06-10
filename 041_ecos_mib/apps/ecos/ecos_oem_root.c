
#include "ecos_oem_root.h"
#include "ecos_oem_database.h"
#include "ecos_configset.h"
#include "ecos_mib_functions.h"
#include "ecos_timer.h"
/****************************************************************************
 *  define top struct and node.
 ****************************************************************************/

ConfigSet_t g_ConfigSet;                // system all configs saved in this.
ConfigSet_t g_ConfigSetBackup;          // backup last configs.

bool_t g_exec_first_flag  = TRUE;       // TRUE : exec anyhow (this is first time exec).
                                        // FALSE: exec when data changed.

bool_t g_exec_parent_flag = FALSE;      // TRUE : must exec, because parent has changed.
                                        // FALSE: exec when data changed.

/****************************************************************************
 *  ecos root config timer define.
 ****************************************************************************/

static ecos_timer_handle s_timer_root_config = ECOS_TIMER_FUNCTION_HANDLE_INVALID;

/****************************************************************************
 *  compare data whether changed.
 ****************************************************************************/

int exec_memcmp(void *mem_addr, unsigned int mem_size)
{
    return memcmp(
        (char *)mem_addr,
        (char *)mem_addr + (size_t)&g_ConfigSetBackup - (size_t)&g_ConfigSet,
        mem_size);
}

int exec_strncmp(void *mem_addr, unsigned int mem_size)
{
    return strncmp(
        (char *)mem_addr,
        (char *)mem_addr + (size_t)&g_ConfigSetBackup - (size_t)&g_ConfigSet,
        mem_size);
}


/****************************************************************************
 *  exec all functions.
 ****************************************************************************/

static void root_config(void *arg)
{
    /**************************************************
     * top: set parent flag to FALSE, clear status.
     **************************************************/ 

    g_exec_parent_flag = FALSE;
    /**************************************************
     * top: when some major node changed.
     *      must reboot system.
     **************************************************/ 
    

    /**************************************************
     * setting: setting all function modules.
     **************************************************/
    ecos_mib_system_exec(NULL);     // exec ecos_mib_system.c

    /**************************************************
     * finish: backup new configset, set exec flag.
     **************************************************/ 
	
    memcpy(&g_ConfigSetBackup, &g_ConfigSet, sizeof(g_ConfigSet));

    if (g_exec_first_flag == TRUE)
        g_exec_first_flag = FALSE;

    //	ecos_update_mfg_html();
    /**************************************************
     * end: we wish you good luck, not to be bad fuck.
    **************************************************/

}

void ecos_mib_root_exec(void *p_arg)
{
    /**************************************************
     * start timer setting root config.
     **************************************************/ 
    
    // 
    // first config system.
    // 
    if (g_exec_first_flag == TRUE)
    {
        printf("first config system.\n");
        root_config(NULL); 
    }

    // 
    // runtime config system.
    // 
    ecos_timer_function_set(s_timer_root_config, root_config, NULL, 2, 1);
}



int ecos_oem_root_init(void)
{

    // 
    // register timers what will needed.
    // 
    s_timer_root_config = ecos_timer_function_add("system root configure", NULL, NULL, 0, 0);
    
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

    ecos_config_file_load(ECOS_CONFIG_FILE);

    ecos_mib_system_init();         // init ecos_mib_system.c

    ecos_mib_root_exec(NULL);

    return 0;
}

void ecos_oem_root_free(void)
{
    // 
    // free mib parser api for client.
    // 
    ecos_mib_parser_client_free();
}