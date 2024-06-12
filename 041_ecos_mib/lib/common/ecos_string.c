
#include "ecos_type.h"
#include "ecos_string.h"

// IP converter routines
void IP2Str(char *szIP, in_addr_t ulIP)
{
    ulIP = htonl(ulIP);
    strcpy(szIP, inet_ntoa(*(struct in_addr*)&ulIP));
}

bool_t Str2IP(in_addr_t *pulIP, const char *cszIP)
{
    struct in_addr  Output;
    if (inet_aton(cszIP, &Output))
    {
        *pulIP = ntohl(Output.s_addr);
        return TRUE;
    }
    return FALSE;
}


// is ip address
bool_t IsValidIP(in_addr_t ulIP)
{
    uint8_t uchFirstOctet;
    uint8_t uchLastOctet;


    uchFirstOctet   = (uint8_t)((ulIP>>24)&0x000000FFL);
    uchLastOctet    = (uint8_t)(ulIP&0x000000FFL);

    if (uchFirstOctet<1)
    {
        printf("First octet of IP must be more than 1.\n");
        return FALSE;
    }
    if (uchFirstOctet>223)
    {
        printf("First octet of IP must be less than 223.\n");
        return FALSE;
    }
    if (uchFirstOctet==127)
    {
        printf("First octet 127 is reserved for loop back\n");
        return FALSE;
    }

    return TRUE;
}