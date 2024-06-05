#ifndef __ECOS_SHASH32_H__
#define __ECOS_SHASH32_H__

#include <stdio.h>

/****************************************************************************
 *
 * DESCRIPTION:
 *	
 * <->  +--- node[0] -> next1 -> next2 -> ..... -> nextx -> NULL
 *  |   |
 *  R   +--- node[1] -> next1 -> ..... -> nextx -> NULL
 *  O   |
 *  W   +--- node[2] -> NULL
 *  |   |
 *  C   +--- node[3] -> next1 -> ..... -> NULL
 *  O   |
 *  U   +--- node[4] -> next1 -> next2 -> next3 -> next4 -> NULL
 *  N   |
 *  T   +--- .......
 *  |   |
 * <->  +--- node[row_count-1] -> .... -> NULL
 *
 * NOTE:
 *  one row, first node do not contain node info, only for search.
 *
 ****************************************************************************/


typedef struct ecos_shash32_node_t
{
    unsigned int        key32;          // 32 bit key.
    struct ecos_shash32_node_t           // ----------
                        *next;          // node next.
    void                *arg;           // arg ptr of node.
} ecos_shash32_node_t;

typedef struct ecos_shash32_t
{
    unsigned int        row_count;      // simple hash row count.
    ecos_shash32_node_t  *node;          // node list, malloc(row_count).
} ecos_shash32_t;

ecos_shash32_node_t* ecos_shash32_node_insert(ecos_shash32_t *shash, unsigned int insert, void *arg);
void ecos_shash32_node_remove(ecos_shash32_t *shash, unsigned int remove);
ecos_shash32_node_t* ecos_shash32_node_search(ecos_shash32_t *shash, unsigned int search);
ecos_shash32_t* ecos_shash32_create(unsigned int row_count);
void ecos_shash_destory(ecos_shash32_t* shash);

#endif //__ECOS_SHASH32_H__
