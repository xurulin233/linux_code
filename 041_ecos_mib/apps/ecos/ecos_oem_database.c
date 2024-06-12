#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "ecos_oem_database.h"
#include "ecos_configset.h"
#include "ecos_addr_rule.h"

/****************************************************************************
 *  extern base functions: load.
 ****************************************************************************/

void ecos_load_common(ecos_config_file_t *config_file, void *arg)
{
    ecos_mib_shash_reg_t *mib_node = (ecos_mib_shash_reg_t *)arg;
    unsigned char *real_addr;

    // 
    // get real addr of gConfigSet.
    //  
    real_addr = mib_node->rule_fn(mib_node->base_addr, mib_node->base_size, config_file->index);
    if (ECOS_ADDR_ERROR == real_addr) return;

    // 
    // set new data to gConfigSet.
    //
    if (real_addr != NULL)
    {
        if ((mib_node->type == ECOS_CF_IP) && (config_file->len == sizeof(uint32_t)))
        {
            uint32_t ipaddr = ntohl(*(uint32_t *)config_file->pdata);
            memcpy(real_addr, &ipaddr, config_file->len);
        }
        else
        {
            memcpy(real_addr, config_file->pdata, config_file->len);
        }

        return;
    }

    return;
}



int ecos_config_file_load(void *arg)
{
    ecos_mib_shash_reg_t *mib_node;
    ecos_config_file_t   *config_file;

    // 
    // open save db file.
    // 
    if (ecos_config_file_open((char *)arg, "r") != 0)
    {
        return ECOS_CONFIG_FILE_LOAD_FAILED;
    }

    // 
    // load all mib by register order list.
    // 
    while (ECOS_CONFIG_FILE_EOF != (config_file=ecos_config_file_load_data()))
    {
        // 
        // read no used data.
        // 
        if (config_file == NULL)    continue;

        // 
        // get mib_node by mib desc.
        // 
        mib_node = ecos_mib_find_by_desc(config_file->key);
  
        // 
        // can not find mib.
        // 
        if (mib_node == NULL)       continue;

         // 
        // check whether need load, don't need: continue.
        // 
        if ( (mib_node->load_fn == NULL)
        ||  ((mib_node->ext_flag & ECOS_EXT_DB_SAVE ) == 0)
        )
        {
            continue;
        }

        // 
        // load config value.
        // 
        mib_node->load_fn(config_file, mib_node);


    }

    // 
    // close db file.
    // 
    ecos_config_file_close();
    return ECOS_CONFIG_FILE_LOAD_SUCCESS;

}