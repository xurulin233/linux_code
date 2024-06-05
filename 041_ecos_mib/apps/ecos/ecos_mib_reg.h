#ifndef __ECOS_MIB_REG_H__
#define __ECOS_MIB_REG_H__

#include "ecos_oem_mib_parser.h"
#include "ecos_config_file.h"


#define ECOS_PERMISSION_SUPER       0x80000000
#define ECOS_PERMISSION_ADMIN       0x00000001
#define ECOS_PERMISSION_USER        0x00000002
#define ECOS_PERMISSION_DEMO        0x00000004
#define ECOS_PERMISSION_ALL         ( ECOS_PERMISSION_SUPER     \
                                    | ECOS_PERMISSION_ADMIN     \
                                    | ECOS_PERMISSION_USER      \
                                    | ECOS_PERMISSION_DEMO      \
                                    )


typedef ecos_exchange_result_t (*mib_parser_fn)(ECOS_MIB_PARSER_PARAMETERS);

typedef void (*ecos_db_save_fn)(ecos_config_file_t* config_file, void *arg, int32_t flag);
typedef void (*ecos_db_load_fn)(ecos_config_file_t *config_file, void *arg);

typedef unsigned char* (*ecos_addr_rule_fn)
    (unsigned char *base_addr, unsigned int base_size, unsigned char index);

typedef struct ecos_mib_shash_reg_t
{
    char                desc[64];       // mib description.
    ecos_config_file_type_e             // ................
                        type;           // mib type for save/load.
    unsigned char       *base_addr;     // node base addr.
    unsigned int        base_size;      // base size of node
    mib_parser_fn       parser_fn;      // mib parser function.
    ecos_addr_rule_fn   rule_fn;        // calculate real addr.
    unsigned int        permission;     // mib permission.
    unsigned int        ext_flag;       // extern flag, eg: save mode.
    ecos_db_save_fn     save_fn;        // mib database save.
    ecos_db_load_fn     load_fn;        // mib database load.
} ecos_mib_shash_reg_t;

#define ECOS_REG_OK                 0
#define ECOS_REG_ERROR_MIB_USED     1

#define ECOS_REG_BASE(base_addr, base_size) \
    (unsigned char *)base_addr, base_size

#define ECOS_REG_NODE(base_node) \
    (unsigned char *)&base_node, sizeof(base_node)

int ecos_mib_register(
    unsigned int            mib, 
    char                    *desc, 
    ecos_config_file_type_e type, 
    unsigned char           *base_addr,
    unsigned int            base_size,
    mib_parser_fn           parser_fn, 
    ecos_addr_rule_fn       rule_fn,
    ecos_db_save_fn         save_fn,
    ecos_db_load_fn         load_fn,
    unsigned int            permission, 
    unsigned int            ext_flag
    );

void ecos_mib_unregister(unsigned int mib);

void ecos_mib_show(void);

ecos_mib_shash_reg_t* ecos_mib_find(unsigned int mib);

ecos_mib_shash_reg_t* ecos_mib_find_by_desc(char *sz_desc);

int ecos_mib_reg_init(void);


void ecos_mib_reg_free(void);

typedef struct ecos_mib_reg_order_t
{
    unsigned int        mib;            // mib what registered.
    struct ecos_mib_reg_order_t         // ...
    /*--------*/        *next;          // next mib.
} ecos_mib_reg_order_t;

ecos_mib_reg_order_t* ecos_get_mib_register_order(void);

void ecos_mib_register_order_show(void);

#endif //__ECOS_MIB_REG_H__
