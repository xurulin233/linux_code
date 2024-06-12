#ifndef __ECOS_CONFIGSET_H__
#define __ECOS_CONFIGSET_H__

#include "ecos_type.h"

typedef struct ConfigIPSet_t
{
    uint32_t            ulIPType;                           // MIB_NET_TYPE
    // static IP
    in_addr_t           inIPAddress;                        // MIB_NET_IP
    in_addr_t           inNetmask;                          // MIB_NET_MASK
    in_addr_t           inGateway;                          // MIB_NET_GATEWAY
    in_addr_t           inDnsPrimary;                       // MIB_NET_DNS_PRIMARY
    in_addr_t           inDnsSecondary;                     // MIB_NET_DNS_SECONDARY
} ConfigIPSet_t;


typedef struct ConfigSet_t
{
    // system
    uint32_t            ulProduct;                    // MIB_PRODUCT
    char                szProductName[32];            // MIB_PRODUCT_NAME
    ConfigIPSet_t       IPSets;                        //MIB_NAT_ENABLE
} ConfigSet_t;

extern ConfigSet_t g_ConfigSet;
extern ConfigSet_t g_ConfigSetBackup;

#endif // __ECOS_CONFIGSET_H__