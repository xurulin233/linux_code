#include"shash32.h"
#include <stdlib.h>

/****************************************************************************
 * node : search / insert / remove
 ****************************************************************************/

shash32_node_t* shash32_node_search(shash32_t *shash, unsigned int search)
{
    unsigned int       index = search % shash->row_count;
    shash32_node_t *node = shash->node[index].next;

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

shash32_node_t* shash32_node_insert(shash32_t *shash, unsigned int insert, void *arg)
{
    unsigned int       index     = insert % shash->row_count;
    shash32_node_t *node_end = &shash->node[index];
    shash32_node_t *node_new;

    if (shash32_node_search(shash, insert) != NULL)
        return NULL;

    // 
    // find end node of one row.
    // 
    while (node_end->next)  node_end = node_end->next;

    // 
    // new one node, and init it.
    // 
    node_new = (shash32_node_t *)malloc(sizeof(shash32_node_t));
    node_new->key32 = insert;
    node_new->arg   = arg;
    node_new->next  = NULL;

    // 
    // insert node_new.
    // 
    node_end->next = node_new;

    return node_new;
}

void shash32_node_remove(shash32_t *shash, unsigned int remove)
{
    unsigned int       index      = remove % shash->row_count;
    shash32_node_t *node      = &shash->node[index];
    shash32_node_t *node_next = node->next;

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
shash32_t* shash32_create(unsigned int row_count)
{
#define SHASH_ROW_COUNT_MIN     (1 <<  4)
#define SHASH_ROW_COUNT_MAX     (1 << 16)

    shash32_t   *shash;
    unsigned int    index;

    // 
    // out of range.
    // 
    if ((row_count<SHASH_ROW_COUNT_MIN) || (row_count>SHASH_ROW_COUNT_MAX))
        return NULL;

    // 
    // malloc shash_t.
    // 
    shash = (shash32_t *)malloc(sizeof(shash32_t));
    shash->row_count = row_count;

    // 
    // malloc shash_node_t.
    // 
    shash->node = (shash32_node_t *)malloc(sizeof(shash32_node_t) * row_count);

    // 
    // init shash_node_t.
    // 
    for (index=0; index<row_count; index++)
    {
        shash->node[index].key32 = 0;
        shash->node[index].next  = NULL;
        shash->node[index].arg   = NULL;
    }

    return shash;
}

void shash_destory(shash32_t* shash)
{
    unsigned int index;
    
    // 
    // free all node.
    // 
    for (index=0; index<shash->row_count; index++)
    {
        shash32_node_t *node = &shash->node[index];

        // 
        // remove all node.
        // 
        while (node->next)  shash32_node_remove(shash, node->next->key32);
    }
    
    // 
    // free these all.
    // 
    free(shash->node);
    free(shash);
}


