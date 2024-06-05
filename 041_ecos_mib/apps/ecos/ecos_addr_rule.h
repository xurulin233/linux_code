#ifndef __ECOS_ADDR_RULE_H__
#define __ECOS_ADDR_RULE_H__

#include "ecos_macro.h"


/****************************************************************************
 *
 * DESCRIPTION:
 *  define macro and struct.
 *
 ****************************************************************************/

#define ECOS_ADDR_ERROR  ((unsigned char *)(~0))

unsigned char* ecos_addr_rule_default(unsigned char *base_addr, unsigned int base_size, unsigned char index);


#endif // __ECOS_ADDR_RULE_H__


