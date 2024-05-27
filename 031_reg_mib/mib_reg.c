#include <stdlib.h>
#include <string.h>
#include "mib_reg.h"
#include "shash32.h"


#define MIB_SHASH_ROW_COUNT     64

static shash32_t *s_mib_shash;


int mib_register(
    unsigned int            mib, 
    char                    *desc, 
    config_file_type_e type, 
    unsigned char           *base_addr,
    unsigned int            base_size,
    mib_parser_fn           parser_fn
)
{

  mib_shash_reg_t *reg_node = (mib_shash_reg_t *)malloc(sizeof(mib_shash_reg_t));

    // 
    // init mib_shash_reg_t node value.
    // 
    strncpy(reg_node->desc, desc, sizeof(reg_node->desc));
    reg_node->type        = type;
    reg_node->base_addr   = base_addr;
    reg_node->base_size   = base_size;
    reg_node->parser_fn   = parser_fn;

    // 
    // insert mib, reg_node, to hash table.
    // 
    if (NULL == shash32_node_insert(s_mib_shash, mib, reg_node))
    {
         return REG_ERROR_MIB_USED;
    }

    return REG_OK;

}

void mib_unregister(unsigned int mib)
{
    shash32_node_t *del_node = shash32_node_search(s_mib_shash, mib);

    // 
    // find nill.
    // 
    if (del_node == NULL)
        return;

    // 
    // free arg memery.
    // 
    free(del_node->arg);

    // 
    // remove hash node.
    // 
    shash32_node_remove(s_mib_shash, mib);
}

void ecos_mib_show(void)
{
    unsigned int index;

    printf("mib_list:\n");

    // 
    // show row.
    // 
    for (index=0; index<s_mib_shash->row_count; index++)
    {
        shash32_node_t *node = s_mib_shash->node[index].next;

        // 
        // show row hash list.
        // 
        printf("[ table %03d ]:\n", index);
        while (node)
        {
            mib_shash_reg_t *reg_t = (mib_shash_reg_t *)node->arg;
            printf("  --> [mib=0x%08x, desc=%s, parser_fn=%p, ]\n", 
                       node->key32, reg_t->desc, reg_t->parser_fn);
            node = node->next;
        }
    }
    printf("\n");
}


int mib_reg_init(void)
{

    // 
    // create mib shash table.
    // 
    s_mib_shash = shash32_create(MIB_SHASH_ROW_COUNT);

    return (s_mib_shash!=NULL) ? 0 : -1;
}

void mib_reg_free(void)
{
    unsigned int index;

    // 
    // free arg memerys.
    // 
    for (index=0; index<s_mib_shash->row_count; index++)
    {
       shash32_node_t *node = s_mib_shash->node[index].next;

        while (node)
        {
            free(node->arg);
            node = node->next;
        }
    }

    // 
    // destory mib shash table.
    // 
    shash_destory(s_mib_shash);
    s_mib_shash = NULL;
}
