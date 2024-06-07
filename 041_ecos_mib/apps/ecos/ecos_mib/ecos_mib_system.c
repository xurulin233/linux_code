#include <net/if.h> //for struct ifreq
#include <sys/ioctl.h> //for ioctl()
#include <sys/socket.h> //for socket()
#include <sys/types.h> //for SOCK_RAW
#include <netinet/in.h> //for IPPROTO_RAW
#include "ecos_mib_functions.h"
#include "ecos_oem_mib_parser.h"
#include "ecos_oem_root.h"
#include "ecos_mib_reg.h"
#include "ecos_mib.h"
#include "ecos_configset.h"
#include "ecos_addr_rule.h"
#include "ecos_timer.h"
#include "ecos_configset.h"

#define LINK_DEV "ens33"

static ecos_timer_handle s_link_integrity_timer = ECOS_TIMER_FUNCTION_HANDLE_INVALID;

//#########################################################################
// get ethernt port status.
bool_t get_dev_run_state(char *dev)
{
    struct ifreq ifr;
    int    fd;
    if ((fd=socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
        return FALSE;
    strcpy(ifr.ifr_name, dev);
    ioctl(fd, SIOCGIFFLAGS, &ifr);
    close(fd);

    return (ifr.ifr_flags&IFF_RUNNING) != 0;
}

//######################################################################
// link integrity monitor.
static void ecos_link_integrity_monitor(void *arg)
{
    static bool_t g_last_link_status = TRUE;
    static bool_t g_curr_link_status = TRUE; 
    g_curr_link_status = get_dev_run_state(LINK_DEV);
    
    if (g_curr_link_status != g_last_link_status)
        {
            g_last_link_status    = g_curr_link_status;
            printf("The wired link has changed and the link current state is %s\n",g_curr_link_status?"TRUE":"FALSE");
        }
}

/****************************************************************************
 *  mib functions
 ****************************************************************************/
ECOS_PARSER static ExchangeResult_t ParserProductName(MIB_PARSER_PARAMETERS)
{
    switch (Request)
    {
        case CONF_REQUEST_FIELD_INFO:

            return CONF_RESULT_OK;

	case CONF_REQUEST_READ:
        memcpy((char *)*ppResp, (char *)pCfgField, ulCfgSize);
        (*ppResp) += ulCfgSize;
        return CONF_RESULT_OK;

    case CONF_REQUEST_WRITE:
            if (usRequestLength > ulCfgSize)
                {
                    return CONF_RESULT_DATA_TOO_LONG;
                }
                pReq[ulCfgSize-1] = '\0';
                if (0 == memcmp((char *)pReq, (char *)pCfgField, usRequestLength))
                {
                    return CONF_RESULT_OK;
                }
                
                memcpy((char *)pCfgField, (char *)pReq, usRequestLength);
                ecos_mib_root_exec(NULL);
                return CONF_RESULT_OK;
        default:
            return CONF_RESULT_REQUEST_NOT_HANDLE;
    }
    return CONF_RESULT_REQUEST_NOT_HANDLE;
}

/****************************************************************************
 *  extern functions
 ****************************************************************************/

void ecos_mib_system_exec(void *p_arg)
{
    //
    // when ap name changed,upgrade wins.
    //
    ECOS_EXEC_START(exec_memcmp, g_ConfigSet.szProductName)
    {
        printf("szProductName is changed %s\n",g_ConfigSet.szProductName);
    }
    ECOS_EXEC_END(g_ConfigSet.szProductName)

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

void ecos_mib_system_init(void)
{
    // 
    // start link integrity function timer.
    // 
    s_link_integrity_timer = ecos_timer_function_add("check link integrity", ecos_link_integrity_monitor, 
                                                        NULL, 2,ECOS_TIMER_FUNCTION_COUNT_FOREVER);

}