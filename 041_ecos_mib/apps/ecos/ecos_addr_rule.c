#include "ecos_addr_rule.h"
#include "ecos_configset.h"
#include "ecos_mib_comm.h"

unsigned char* ecos_addr_rule_default(unsigned char *base_addr, unsigned int base_size, unsigned char index)
{
    // 
    // only return base addr.
    // 
    return base_addr;
}
