
#ifndef __SHASH32_H__
#define __SHASH32_H__

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

typedef struct shash32_node_t
{
    unsigned int           key32;          // 32 bit key.
    struct shash32_node_t  *next;  // node next.
    void                   *arg;           // arg ptr of node.
} shash32_node_t;

typedef struct shash32_t
{
    unsigned int        row_count;      // simple hash row count.
    shash32_node_t  *node;          // node list, malloc(row_count).
} shash32_t;



shash32_t* shash32_create(unsigned int row_count);

void shash_destory(shash32_t* shash);

shash32_node_t* shash32_node_insert(shash32_t *shash, unsigned int insert, void *arg);

void shash32_node_remove(shash32_t *shash, unsigned int remove);

shash32_node_t* shash32_node_search(shash32_t *shash, unsigned int search);

#endif //__SHASH32_H__




