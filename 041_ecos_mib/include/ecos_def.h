
/*
 * ecos_def.h
 */
#ifndef __ECOS_DEF_H__
#define __ECOS_DEF_H__

typedef enum IPStr_t
{
    IP_STR_MIN_LEN = 8,
    IP_STR_MAX_LEN = 16
} IPStr_t;

typedef enum IPType_t
{
    IP_TYPE_STATIC_IP = 0,
    IP_TYPE_DHCP_CLIENT,
} IPType_t;


#endif// __ECOS_DEF_H__
