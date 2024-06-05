#include <stdlib.h>
#include "ecos_shash32.h"


/****************************************************************************
 * node : search / insert / remove
 ****************************************************************************/
 
ecos_shash32_node_t* ecos_shash32_node_search(ecos_shash32_t *shash, unsigned int search)
{
    unsigned int       index = search % shash->row_count;
    ecos_shash32_node_t *node = shash->node[index].next;

    // 
    // find one row.
    // 
    while (node)
    {
        if (node->key32 == search)
            return node;

        node = node->next;
    }

    return NULL;
}

ecos_shash32_node_t* ecos_shash32_node_insert(ecos_shash32_t *shash, unsigned int insert, void *arg)
{
    unsigned int       index     = insert % shash->row_count;
    ecos_shash32_node_t *node_end = &shash->node[index];
    ecos_shash32_node_t *node_new;

    if (ecos_shash32_node_search(shash, insert) != NULL)
        return NULL;

    // 
    // find end node of one row.
    // 
    while (node_end->next)  node_end = node_end->next;

    // 
    // new one node, and init it.
    // 
    node_new = (ecos_shash32_node_t *)malloc(sizeof(ecos_shash32_node_t));
    node_new->key32 = insert;
    node_new->arg   = arg;
    node_new->next  = NULL;

    // 
    // insert node_new.
    // 
    node_end->next = node_new;

    return node_new;
}

void ecos_shash32_node_remove(ecos_shash32_t *shash, unsigned int remove)
{
    unsigned int       index      = remove % shash->row_count;
    ecos_shash32_node_t *node      = &shash->node[index];
    ecos_shash32_node_t *node_next = node->next;

    // 
    // search and remove node.
    // 
    while (node_next)
    {
        // 
        // remove it.
        // 
        if (node_next->key32 == remove)
        {
            node->next = node_next->next;
            free(node_next);
            return;
        }

        // 
        // search next.
        // 
        node = node_next;
        node_next = node_next->next;
    }
}

/****************************************************************************
 * hash table : create / destory
 ****************************************************************************/

ecos_shash32_t* ecos_shash32_create(unsigned int row_count)
{
#define SHASH_ROW_COUNT_MIN     (1 <<  4)
#define SHASH_ROW_COUNT_MAX     (1 << 16)

    ecos_shash32_t   *shash;
    unsigned int    index;

    // 
    // out of range.
    // 
    if ((row_count<SHASH_ROW_COUNT_MIN) || (row_count>SHASH_ROW_COUNT_MAX))
        return NULL;

    // 
    // malloc ecos_shash_t.
    // 
    shash = (ecos_shash32_t *)malloc(sizeof(ecos_shash32_t));
    shash->row_count = row_count;

    // 
    // malloc ecos_shash_node_t.
    // 
    shash->node = (ecos_shash32_node_t *)malloc(sizeof(ecos_shash32_node_t) * row_count);

    // 
    // init ecos_shash_node_t.
    // 
    for (index=0; index<row_count; index++)
    {
        shash->node[index].key32 = 0;
        shash->node[index].next  = NULL;
        shash->node[index].arg   = NULL;
    }

    return shash;
}

void ecos_shash_destory(ecos_shash32_t* shash)
{
    unsigned int index;
    
    // 
    // free all node.
    // 
    for (index=0; index<shash->row_count; index++)
    {
        ecos_shash32_node_t *node = &shash->node[index];

        // 
        // remove all node.
        // 
        while (node->next)  ecos_shash32_node_remove(shash, node->next->key32);
    }
    
    // 
    // free these all.
    // 
    free(shash->node);
    free(shash);
}
