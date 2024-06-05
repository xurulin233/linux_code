#include <stdlib.h>
#include <string.h>
#include "ecos_mib_reg.h"
#include "ecos_shash32.h"

#define MIB_SHASH_ROW_COUNT     64

static ecos_shash32_t *s_mib_shash;
static ecos_mib_reg_order_t s_reg_order_head;
static ecos_mib_reg_order_t *sp_reg_order_end;

static void free_mib_register_order(void)
{
    ecos_mib_reg_order_t *p_order = s_reg_order_head.next;
    ecos_mib_reg_order_t *p_order_free;

    // 
    // free order list.
    // 
    while (p_order != NULL)
    {
        p_order_free = p_order;
        p_order = p_order->next;

        free(p_order_free);
    }

    s_reg_order_head.next = NULL;
}

static void add_mib_register_order(unsigned int mib)
{
    ecos_mib_reg_order_t *new_order = s_reg_order_head.next;

    // 
    // find mib whether in list.
    // 
    while (new_order != NULL)
    {
        if (new_order->mib == mib)
            return;
        new_order = new_order->next;
    }

    // 
    // malloc new order.
    // 
    new_order = (ecos_mib_reg_order_t *)malloc(sizeof(ecos_mib_reg_order_t));

    // 
    // init new order value.
    // 
    new_order->mib  = mib;
    new_order->next = NULL;

    // 
    // insert to list.
    // 
    sp_reg_order_end->next = new_order;
    sp_reg_order_end       = new_order;
}

ecos_mib_reg_order_t* ecos_get_mib_register_order(void)
{
    // 
    // return order list head.
    // 
    return &s_reg_order_head;
}

void ecos_mib_register_order_show(void)
{
    ecos_mib_reg_order_t *p_order = s_reg_order_head.next;
    unsigned int index = 0;

    printf("ecos_mib_register_order_list:\n");

    // 
    // show order list.
    // 
    while (p_order != NULL)
    {
        printf("[%03d] mib(0x%08x)\n", index++, p_order->mib);
        p_order = p_order->next;
    }
    printf("\n");
}

/****************************************************************************
 *  register / unregister
 ****************************************************************************/

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
    )
{
    ecos_mib_shash_reg_t *reg_node = (ecos_mib_shash_reg_t *)malloc(sizeof(ecos_mib_shash_reg_t));

    // 
    // check mib desc whether exist.
    // 
    if (NULL != ecos_mib_find_by_desc(desc))
    {
        char sz_bug[256];

        sprintf(sz_bug, "mib(%s): desc was exist.", desc);
        printf("%s\n", sz_bug);
    }

    // 
    // init ecos_mib_shash_reg_t node value.
    // 
    strncpy(reg_node->desc, desc, sizeof(reg_node->desc));
    reg_node->type        = type;
    reg_node->base_addr   = base_addr;
    reg_node->base_size   = base_size;
    reg_node->parser_fn   = parser_fn;
    if (rule_fn == NULL)
    {
        char sz_bug[256];

        sprintf(sz_bug, "mib(%s): must insert addr rule functions.", reg_node->desc);
        printf("%s\n", sz_bug);
    }
    else
        reg_node->rule_fn = rule_fn;
    reg_node->save_fn     = save_fn;
    reg_node->load_fn     = load_fn;
    reg_node->permission  = permission;
    reg_node->ext_flag    = ext_flag;

    // 
    // insert mib, reg_node, to hash table.
    // 
    if (NULL == ecos_shash32_node_insert(s_mib_shash, mib, reg_node))
    {
         printf("mib(0x%08x, %s) has already insert\n", mib, desc);
         return ECOS_REG_ERROR_MIB_USED;
    }

    // 
    // insert mib to order list.
    // 
    add_mib_register_order(mib);

    return ECOS_REG_OK;
}

void ecos_mib_unregister(unsigned int mib)
{
    ecos_shash32_node_t *del_node = ecos_shash32_node_search(s_mib_shash, mib);

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
    ecos_shash32_node_remove(s_mib_shash, mib);
}

void ecos_mib_show(void)
{
    unsigned int index;

    printf("ecos_mib_list:\n");

    // 
    // show row.
    // 
    for (index=0; index<s_mib_shash->row_count; index++)
    {
        ecos_shash32_node_t *node = s_mib_shash->node[index].next;

        // 
        // show row hash list.
        // 
        printf("[ table %03d ]:\n", index);
        while (node)
        {
            ecos_mib_shash_reg_t *reg_t = (ecos_mib_shash_reg_t *)node->arg;
            printf("  --> [mib=0x%08x, desc=%s, parser_fn=%p, permission=0x%08x, ext_flag=0x%08x]\n", 
                       node->key32, reg_t->desc, reg_t->parser_fn, reg_t->permission, reg_t->ext_flag);
            node = node->next;
        }
    }
    printf("\n");
}

ecos_mib_shash_reg_t* ecos_mib_find(unsigned int mib)
{
    ecos_shash32_node_t *find_node = ecos_shash32_node_search(s_mib_shash, mib);

    if (find_node != NULL)
        return (ecos_mib_shash_reg_t*)find_node->arg;
        
    return NULL;
}

ecos_mib_shash_reg_t* ecos_mib_find_by_desc(char *sz_desc)
{
    unsigned int index;

    // 
    // find desc in all table.
    // 
    for (index=0; index<s_mib_shash->row_count; index++)
    {
        ecos_shash32_node_t *node = s_mib_shash->node[index].next;

        // 
        // find row hash list.
        // 
        while (node)
        {
            ecos_mib_shash_reg_t *reg_t = (ecos_mib_shash_reg_t *)node->arg;
            if (0 == strcmp(reg_t->desc, sz_desc)) return reg_t;
            node = node->next;
        }
    }

    return NULL;
}

/****************************************************************************
 *  init / free
 ****************************************************************************/

int ecos_mib_reg_init(void)
{
    // 
    // init register order head.
    // 
    s_reg_order_head.mib  = 0x00;
    s_reg_order_head.next = NULL;
    sp_reg_order_end = &s_reg_order_head;
    
    // 
    // create mib shash table.
    // 
    s_mib_shash = ecos_shash32_create(MIB_SHASH_ROW_COUNT);

    return (s_mib_shash!=NULL) ? 0 : -1;
}

void ecos_mib_reg_free(void)
{
    unsigned int index;

    // 
    // free register order head.
    // 
    free_mib_register_order();
    
    // 
    // free arg memerys.
    // 
    for (index=0; index<s_mib_shash->row_count; index++)
    {
        ecos_shash32_node_t *node = s_mib_shash->node[index].next;

        while (node)
        {
            free(node->arg);
            node = node->next;
        }
    }

    // 
    // destory mib shash table.
    // 
    ecos_shash_destory(s_mib_shash);
    s_mib_shash = NULL;
}
