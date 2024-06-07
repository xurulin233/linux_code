#ifndef __ECOS_OEM_ROOT_H__
#define __ECOS_OEM_ROOT_H__

#include "ecos_oem_mib_parser.h"
#include "ecos_mib_comm.h"
#include "ecos_macro.h"


/****************************************************************************
 *
 * DESCRIPTION:
 *  check configset node whether changed.
 *
 ****************************************************************************/

extern bool_t g_exec_first_flag;    // first exec flag, TRUE: must run all exec functions.
extern bool_t g_exec_parent_flag;   // parent exec flag, TRUE: must run child exec functions.

/****************************************************************************
 *
 * DESCRIPTION:
 *  support tree exec macro.
 *
 * FORMAT: (must be)
 *  example 1:
 *   ECOS_EXEC_START(opt, node1)
 *   {
 *    exec your functions.
 *   }
 *   ECOS_EXEC_END(node1)
 *
 *  example 2:
 *   ECOS_EXEC_START(opt, node1)
 *   {
 *    exec your functions.
 *   }
 *
 *   ECOS_EXEC_START(opt, node2)
 *   {
 *    exec your functions.
 *   }
 *   ECOS_EXEC_END(node2)
 *
 *   ECOS_EXEC_END(node1) 
 *
 * note:
 *  one ECOS_EXEC_START must add { } and ECOS_EXEC_END.
 ****************************************************************************/

#define ECOS_EXEC_START(opt, config_node)       \
{                                               \
    bool_t parent_backup = g_exec_parent_flag;  \
    if ( (TRUE == g_exec_first_flag ) ||        \
         (TRUE == g_exec_parent_flag) ||        \
         (0 != opt(&config_node,                \
         sizeof(config_node))) )                \
    {                                           \
        g_exec_parent_flag = TRUE;              \
    }                                           \
    if (g_exec_parent_flag == TRUE)

#define ECOS_EXEC_END(config_node)              \
    g_exec_parent_flag = parent_backup;         \
}

/****************************************************************************
 *
 * DESCRIPTION:
 *  compare data whether changed.
 *
 ****************************************************************************/

int exec_memcmp(void *mem_addr, unsigned int mem_size);
int exec_strncmp(void *mem_addr, unsigned int mem_size);

/****************************************************************************
 * FUNCTION:
 *  ecos_mib_root_exec
 *
 * DESCRIPTION:
 *  a very important function that setting full system.
 *
 * PARAMETERS:
 *  p_arg -- set NULL.
 *
 * RETURNS:
 *  void.
 *
 ****************************************************************************/

void ecos_mib_root_exec(void *p_arg);

/****************************************************************************
 * FUNCTION:
 *  ecos_oem_root_init
 *
 * DESCRIPTION:
 *  init oem root functions.
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  -1: failed.
 *   0: successed.
 *
 ****************************************************************************/
int ecos_oem_root_init(void);

/****************************************************************************
 * FUNCTION:
 *  ecos_oem_root_free
 *
 * DESCRIPTION:
 *  free oem root functions.
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  void.
 *
 ****************************************************************************/
void ecos_oem_root_free(void);
#endif // __ECOS_OEM_ROOT_H__
