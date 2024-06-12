#ifndef __ECOS_MIB_FUNCTIONS_H__
#define __ECOS_MIB_FUNCTIONS_H__

/****************************************************************************
 *
 * DESCRIPTION:
 *  extern all mib init functions.
 *
 ****************************************************************************/
void ecos_mib_system_init(void);
void ecos_mib_net_init(void);

/****************************************************************************
 *
 * DESCRIPTION:
 *  extern all register mib functions.
 *
 ****************************************************************************/

void ecos_mibs_system_register(void);
void ecos_mibs_net_register(void);

/****************************************************************************
 *
 * DESCRIPTION:
 *  extern all mib exec functions.
 *
 ****************************************************************************/

void ecos_mib_system_exec(void *p_arg);
void ecos_mib_net_exec(void *p_arg);

#endif // __ECOS_MIB_FUNCTIONS_H__
