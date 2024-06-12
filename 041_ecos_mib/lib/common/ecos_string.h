// ecos_string.h
//

#ifndef	__ECOS_STRING_H__
#define	__ECOS_STRING_H__

#include "ecos_type.h"

void IP2Str(char *szIP, in_addr_t ulIP);
bool_t Str2IP(in_addr_t *pulIP, const char *cszIP);
bool_t IsValidIP(in_addr_t ulIP);

#endif// __ECOS_STRING_H__
