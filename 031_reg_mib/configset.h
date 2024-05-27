
#ifndef __CONFIGSET_H__
#define __CONFIGSET_H__

typedef struct ConfigSet_t
{
    // system
    unsigned int            ulProduct;                          // MIB_PRODUCT
    char                szProductName[32];            // MIB_PRODUCT_NAME
}ConfigSet_t;

extern ConfigSet_t g_ConfigSet;

#endif // __CONFIGSET_H__