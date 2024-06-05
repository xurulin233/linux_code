/*
 * ecos_mib.h
 */
#ifndef __ECOS_MIB_H__
#define __ECOS_MIB_H__

#define CONFIG_REVISION     "1.0.0"
#define MIB_WG302_BASE      0x33100000
#define MIB_RANGE_START(X)  (0x00010000 * X)

typedef enum MIBSystem_t
{
    MIB_SYSTEM_FIRST = MIB_WG302_BASE + MIB_RANGE_START(0),
    MIB_PRODUCT = MIB_SYSTEM_FIRST,
    MIB_SERIAL_NUMBER,
    MIB_PLATFORM_TEST,
    MIB_FIRMWARE_TEST,  
    MIB_PRODUCT_NAME,
	MIB_SYSTEM_LAST
} MIBSystem_t;






#endif//__ECOS_MIB_H__
