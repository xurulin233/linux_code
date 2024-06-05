#ifndef __ECOS_CONFIGSET_H__
#define __ECOS_CONFIGSET_H__

#include "ecos_type.h"

typedef struct ConfigSet_t
{
    // system
    uint32_t            ulProduct;                          // MIB_PRODUCT
    char                szProductName[32];            // MIB_PRODUCT_NAME

} ConfigSet_t;

extern ConfigSet_t g_ConfigSet;
extern ConfigSet_t g_ConfigSetBackup;

#endif // __ECOS_CONFIGSET_H__