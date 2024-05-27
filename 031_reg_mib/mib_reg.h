#ifndef __MIB_REG_H__
#define __MIB_REG_H__


#define REG_OK                 0
#define REG_ERROR_MIB_USED     1

#define REG_NODE(base_node) \
    (unsigned char *)&base_node, sizeof(base_node)


typedef void (*mib_parser_fn)(void);

typedef enum config_file_type_e
{
    CF_U32,                    // unsigned int.
    CF_IP,                     // unsigned int(ip).
    CF_MAC,                    // char [6].
    CF_STRING,                 // string.
    CF_HEX,                    // hex format.
} config_file_type_e;


typedef struct mib_shash_reg_t
{
    char                desc[64];       // mib description.
    config_file_type_e             // ................
                        type;           // mib type for save/load.
    unsigned char       *base_addr;     // node base addr.
    unsigned int        base_size;      // base size of node
    mib_parser_fn       parser_fn;      // mib parser function.
} mib_shash_reg_t;

int mib_register(
    unsigned int            mib, 
    char                    *desc, 
    config_file_type_e type, 
    unsigned char           *base_addr,
    unsigned int            base_size,
    mib_parser_fn           parser_fn
    );

int mib_reg_init(void);
void mib_reg_free(void);
void ecos_mib_show(void);
#endif //__MIB_REG_H__