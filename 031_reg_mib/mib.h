#ifndef __MIB_H__
#define __MIB_H__

#define MIB_BASE      0x33100000
#define MIB_RANGE_START(X)  (0x00010000 * X)

typedef enum MIBSystem_t
{
    MIB_SYSTEM_FIRST = MIB_BASE + MIB_RANGE_START(0),
    MIB_PRODUCT = MIB_SYSTEM_FIRST,
    MIB_PRODUCT_NAME,
} MIBSystem_t;


#endif//__MIB_H__
