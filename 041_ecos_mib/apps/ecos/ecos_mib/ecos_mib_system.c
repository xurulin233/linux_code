
#include "ecos_mib_functions.h"
#include "ecos_oem_mib_parser.h"
#include "ecos_oem_root.h"
#include "ecos_mib_reg.h"
#include "ecos_mib.h"
#include "ecos_configset.h"
#include "ecos_addr_rule.h"


ECOS_PARSER static ExchangeResult_t ParserProductName(MIB_PARSER_PARAMETERS)
{
    switch (Request)
    {
        case CONF_REQUEST_FIELD_INFO:

            return CONF_RESULT_OK;

        case CONF_REQUEST_READ:
            // 
            // setting response data value, offset response.
            // 
            printf("read ParserProductName\n");
            memcpy(*ppResp, pCfgField, ulCfgSize);
            (*ppResp) += ulCfgSize;
            return CONF_RESULT_OK;

        case CONF_REQUEST_WRITE:

             return CONF_RESULT_OK;

        default:
            return CONF_RESULT_REQUEST_NOT_HANDLE;
    }
    return CONF_RESULT_REQUEST_NOT_HANDLE;
}



void ecos_mibs_system_register(void)
{
    ecos_mib_register
    (
        MIB_PRODUCT_NAME, "ProductName", ECOS_CF_STRING, 
        ECOS_REG_NODE(g_ConfigSet.szProductName), 
        ParserProductName, ecos_addr_rule_default, 
        NULL, NULL, 
        ECOS_PERMISSION_ALL, ECOS_EXT_NONE
    );

}