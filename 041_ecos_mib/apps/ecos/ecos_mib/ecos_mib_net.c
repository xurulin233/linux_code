//
//ecos_mib_net.c
//

#include "ecos_string.h"
#include "ecos_dev_api.h"
#include "ecos_mib_functions.h"
#include "ecos_oem_mib_parser.h"
#include "ecos_oem_root.h"
#include "ecos_mib_reg.h"
#include "ecos_mib.h"
#include "ecos_configset.h"
#include "ecos_addr_rule.h"
#include "ecos_timer.h"
#include "ecos_configset.h"
#include "ecos_oem_database.h"
#include "ecos_def.h"

#define DEF_IF_ETH "ens33"

/****************************************************************************
 *  extern functions
 ****************************************************************************/

// ecos ipsetting.
void ecos_setting_net_ip(char *dev)
{
    ConfigIPSet_t   *pIPSet;
    pIPSet = &g_ConfigSet.IPSets;

    if (IP_TYPE_DHCP_CLIENT  == g_ConfigSet.IPSets.ulIPType
        || IP_TYPE_STATIC_IP == g_ConfigSet.IPSets.ulIPType)
    {
        // 
        // setting network ip address.
        // 
        InterfaceIPSet(dev, pIPSet->inIPAddress);

    }
    return;
}	


void ecos_upgrade_network_settings(char *dev_name)
{
  //  ECOS_EXEC_START(exec_memcmp, g_ConfigSet.IPSets.ulIPType)

    switch (g_ConfigSet.IPSets.ulIPType)
    {
        case IP_TYPE_STATIC_IP:
            //
            // static: set ip settings.
            //
            ECOS_EXEC_START(exec_memcmp, g_ConfigSet.IPSets)
            {
                ecos_setting_net_ip(dev_name);
                printf("End of static ip settings\n");
            }
            ECOS_EXEC_END("End of static ip settings") 
            break;
        default:
            break;
    }      
    //ECOS_EXEC_END(g_ConfigSet.IPSets.ulIPType)
}


void ecos_mib_net_exec(void *p_arg)
{
    ecos_upgrade_network_settings(DEF_IF_ETH);
}


/****************************************************************************
 *  mib functions
 ****************************************************************************/

ECOS_PARSER static ExchangeResult_t ParserSystemIPType(MIB_PARSER_PARAMETERS)
{
    uint32_t  ulReqData  = *(uint32_t*)pReq;
    uint32_t  *pulCfg    = (uint32_t*)pCfgField;

    switch (Request)
    {
    case CONF_REQUEST_FIELD_INFO:
        return CONF_RESULT_OK;

    case CONF_REQUEST_READ:
        memcpy(*ppResp, pCfgField, ulCfgSize);
        (*ppResp) += ulCfgSize;
        return CONF_RESULT_OK;

    case CONF_REQUEST_WRITE:
        if (usRequestLength < ulCfgSize)
        {
            return CONF_RESULT_DATA_TOO_SHORT;
        }
        if (usRequestLength > ulCfgSize)
        {
            return CONF_RESULT_DATA_TOO_LONG;
        }

        if (ulReqData != *pulCfg)
        {
            *pulCfg = ulReqData;
            ecos_mib_root_exec(NULL);
            return CONF_RESULT_OK;
        }
        return CONF_RESULT_REQUEST_NOT_HANDLE;        
    default:
        return CONF_RESULT_REQUEST_NOT_HANDLE;
    }
    return CONF_RESULT_REQUEST_NOT_HANDLE;
}

ECOS_PARSER static ExchangeResult_t ParserSystemIP(MIB_PARSER_PARAMETERS)
{
    in_addr_t       ulIP = 0;
    ConfigIPSet_t   *pIPSet;
    pIPSet = &g_ConfigSet.IPSets;

    switch (Request)
    {
    case CONF_REQUEST_FIELD_INFO:

        return CONF_RESULT_OK;
    case CONF_REQUEST_READ:
        IP2Str((char *)*ppResp, g_ConfigSet.IPSets.inIPAddress);
        *ppResp += strlen((char *)*ppResp)+1;
        return CONF_RESULT_OK;

    case CONF_REQUEST_WRITE:
        if (usRequestLength < IP_STR_MIN_LEN)
        {
            return CONF_RESULT_DATA_TOO_SHORT;
        }
        
        if (usRequestLength > IP_STR_MAX_LEN)
        {
            return CONF_RESULT_DATA_TOO_LONG;
        }
        
        if (!Str2IP(&ulIP, (char *)pReq) || !IsValidIP(ulIP))
        {
            return CONF_RESULT_DATA_INVALID;
        }
        
        if (pIPSet->inIPAddress != ulIP)
        {
            pIPSet->inIPAddress = ulIP;
            ecos_mib_root_exec(NULL);
            return CONF_RESULT_OK;
        }
        return CONF_RESULT_REQUEST_NOT_HANDLE;        
    default:
        return CONF_RESULT_REQUEST_NOT_HANDLE;
    }
    return CONF_RESULT_REQUEST_NOT_HANDLE;
}


/****************************************************************************
 *  init / free
 ****************************************************************************/

void ecos_mibs_net_register(void)
{

    ecos_mib_register
    (
        MIB_NET_TYPE, "ulIPType", ECOS_CF_U32,
        ECOS_REG_NODE(g_ConfigSet.IPSets.ulIPType),
        ParserSystemIPType, ecos_addr_rule_default,
        NULL, NULL, 
        ECOS_PERMISSION_ALL, ECOS_EXT_DB_SAVE|ECOS_EXT_DB_TAP_SAVE
    );

    ecos_mib_register
    (
        MIB_NET_IP, "inIPAddress", ECOS_CF_IP, 
        ECOS_REG_NODE(g_ConfigSet.IPSets.inIPAddress),
        ParserSystemIP, ecos_addr_rule_default,
        NULL, NULL,
        ECOS_PERMISSION_ALL, ECOS_EXT_DB_SAVE|ECOS_EXT_DB_TAP_SAVE
    ); 


}


void ecos_mib_net_init(void)
{
    printf("ecos_mib_net_init\n");
}
