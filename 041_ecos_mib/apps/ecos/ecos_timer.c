/****************************** Module Header ******************************\
* Module Name: ecos_timer.c
*
\***************************************************************************/

#include <stdlib.h>
#include <stdio.h> // for printf
#include <sys/sysinfo.h>
#include <string.h>
#include "ecos_timer.h"


// ################################################################################
// private struct
// ################################################################################

typedef struct ecos_timer_function_list_t
{
    ecos_timer_handle                       hnd;            // handle id.
    char                                    timer_desc[128];// timer function, note: overflow.
    ecos_fn_timer_function                  func;           // timer function.
    void*                                   p_args;         // timer function args.
    int                                     interval;       // interval(second).
    int                                     count;          // count.
    long                                    last_time;      // previous deal time.
    unsigned int                            suspend;        // whether need suspend.
    struct ecos_timer_function_list_t       *prev;          // prev ptr.
    struct ecos_timer_function_list_t       *next;          // next ptr.
} ecos_timer_function_list_t;

// ################################################################################
// define
// ################################################################################

#define TRUE  1     // internal define. need modify to top include.
#define FALSE 0     // internal define.

#define ECOS_TIMER_FUNCTION_HANDLE_BEGIN      (0x1097C)       // timer handle begin.
#define ECOS_TIMER_FUNCTION_MAX_TIMER_NUM     (64)            // here can limit oem timer numbers

static ecos_timer_function_list_t *s_p_ecos_timer_function_list_head = NULL;
static ecos_timer_function_list_t *s_p_ecos_timer_function_list_tail = NULL;

// ################################################################################
// static
// ################################################################################

static ecos_timer_function_list_t* get_timerlist_ptr_by_handle(ecos_timer_handle hnd)
{
    ecos_timer_function_list_t *p_timer = s_p_ecos_timer_function_list_head;

    //
    // find handle whether in list ecos_timer_function_list_t.
    //
    while ((p_timer!=NULL) && (p_timer->hnd!=hnd))
    {
        p_timer = p_timer->next;
    }

    //
    // RETURN
    // NULL   : can not find handle what in list ecos_timer_function_list_t.
    // NO NULL: return the handle ptr what in list ecos_timer_function_list_t.
    //
    return (((p_timer!=NULL) && (p_timer->hnd==hnd)) ? (p_timer) : (NULL));
}

static ecos_timer_handle get_unused_handle_id(void)
{
    ecos_timer_handle hnd = ECOS_TIMER_FUNCTION_HANDLE_INVALID;

    //
    // get a unused timer id.
    // timers must LE than ECOS_TIMER_FUNCTION_MAX_TIMER_NUM.
    //
    for ( hnd = ECOS_TIMER_FUNCTION_HANDLE_BEGIN;
          hnd < ECOS_TIMER_FUNCTION_HANDLE_BEGIN + ECOS_TIMER_FUNCTION_MAX_TIMER_NUM;
          hnd ++ )
    {
        if (get_timerlist_ptr_by_handle(hnd) == NULL)
        {
            return hnd;
        }
    }
    
    return ECOS_TIMER_FUNCTION_HANDLE_INVALID;
}

// ################################################################################
// loop / show
// ################################################################################

long ecos_get_current_uptime(void)
{
    static struct sysinfo s_sysinfo;

    //
    // get system info.
    //
    sysinfo(&s_sysinfo);

    //
    // return uptime.
    //
    return s_sysinfo.uptime;
}

void ecos_timer_function_loop(void)
{
    static ecos_timer_function_list_t *s_p_timer = NULL;
    static long s_current_uptime = 0;

    //
    // run all timers what in list ecos_timer_function_list_t.
    //
    s_current_uptime = ecos_get_current_uptime();
    s_p_timer = s_p_ecos_timer_function_list_head;
    while (s_p_timer != NULL)
    {
        //
        // run timer function
        //
        if ( (FALSE == s_p_timer->suspend)
          && (NULL  != s_p_timer->func)
          && (s_p_timer->interval > 0)
          && (s_p_timer->count   != 0)
          && (s_current_uptime   >= (s_p_timer->last_time+s_p_timer->interval)) )
        {

            s_p_timer->last_time =  s_current_uptime;
            if (s_p_timer->count != ECOS_TIMER_FUNCTION_COUNT_FOREVER)
            {
                s_p_timer->count--;
            }
            s_p_timer->func(s_p_timer->p_args);

        }
        
        s_p_timer = s_p_timer->next;
    }

}

void ecos_timer_function_show(void)
{
    static ecos_timer_function_list_t *s_p_timer = NULL;
    static unsigned int inum;

    //
    // show all timers what in list ecos_timer_function_list_t.
    //
    printf("cw timer list:\n");
    s_p_timer = s_p_ecos_timer_function_list_head;
    inum = 0;
    while (s_p_timer != NULL)
    {
        printf("[%02d] hnd(%d), func(%p), p_args(%p), interval(%d), count(%d), suspend(%d)\n", 
            ++inum, s_p_timer->hnd, s_p_timer->func, s_p_timer->p_args, 
            s_p_timer->interval, s_p_timer->count, s_p_timer->suspend);

        s_p_timer = s_p_timer->next;
    }
    printf("\n");
}

// ################################################################################
// add / del
// ################################################################################

ecos_timer_handle ecos_timer_function_add(char *timer_desc, ecos_fn_timer_function func, void *p_args, int interval, int count)
{
    ecos_timer_function_list_t *p_timer = (ecos_timer_function_list_t *)malloc(sizeof(ecos_timer_function_list_t));

    //
    // get unused handle id.
    // return invalid when get nil.
    // note: ECOS_TIMER_FUNCTION_MAX_TIMER_NUM.
    //
    p_timer->hnd = get_unused_handle_id();
    if (p_timer->hnd == ECOS_TIMER_FUNCTION_HANDLE_INVALID)
    {
        printf("timer has full, please buy advanced version\n");
        free(p_timer);
        return ECOS_TIMER_FUNCTION_HANDLE_INVALID;
    }

    //
    // config new timer ptr.
    //
    strncpy(p_timer->timer_desc, timer_desc, sizeof(p_timer->timer_desc));
    p_timer->func      = func;
    p_timer->p_args    = p_args;
    p_timer->interval  = interval;
    p_timer->count     = count;
    p_timer->last_time = ecos_get_current_uptime();
    p_timer->suspend   = FALSE;
    p_timer->prev      = s_p_ecos_timer_function_list_tail;
    p_timer->next      = NULL;

    //
    // insert p_timer to list ecos_timer_function_list_t.
    //
    if (s_p_ecos_timer_function_list_tail == NULL)
    {
        s_p_ecos_timer_function_list_head = p_timer;
        s_p_ecos_timer_function_list_tail = s_p_ecos_timer_function_list_head;
    }
    else
    {
        s_p_ecos_timer_function_list_tail->next = p_timer;
        s_p_ecos_timer_function_list_tail       = p_timer;
    }
    
    return p_timer->hnd;
}

unsigned int ecos_timer_function_del(ecos_timer_handle hnd)
{
    ecos_timer_function_list_t *p_timer = NULL;

    //
    // return FALSE when handle not in list ecos_timer_function_list_t.
    //
    if ((p_timer=get_timerlist_ptr_by_handle(hnd)) == NULL)
    {
        printf("can not find handle: 0x%08X\n", hnd);
        return FALSE;
    }

    //
    // remove p_timer from list ecos_timer_function_list_t.
    //
    // **************************************************
    // case A: (s_p_ecos_timer_function_list_head == s_p_ecos_timer_function_list_tail)
    //
    //    +--- s_p_ecos_timer_function_list_head
    //    v
    //    @<-- p_timer
    //    ^
    //    +--- s_p_ecos_timer_function_list_tail
    // 
    // **************************************************
    // case B: (p_timer == s_p_ecos_timer_function_list_head)
    //
    //    +--- s_p_ecos_timer_function_list_head
    //    v
    //    @---@---@---@---@---@---@
    //    ^                       ^
    //    +--- p_timer            +--- s_p_ecos_timer_function_list_tail
    // 
    // **************************************************
    // case C: (p_timer == s_p_ecos_timer_function_list_tail)
    //
    //    +--- s_p_ecos_timer_function_list_head
    //    |
    //    |                       +--- p_timer
    //    v                       V
    //    @---@---@---@---@---@---@
    //                            ^
    //                            +--- s_p_ecos_timer_function_list_tail
    // 
    // **************************************************
    // case D: 
    //
    //    +--- s_p_ecos_timer_function_list_head
    //    |
    //    |           +--- p_timer
    //    v           V
    //    @---@---@---@---@---@---@
    //                            ^
    //                            +--- s_p_ecos_timer_function_list_tail
    // 
    // **************************************************
    //
    if (s_p_ecos_timer_function_list_head == s_p_ecos_timer_function_list_tail)
    {
        s_p_ecos_timer_function_list_head = NULL;
        s_p_ecos_timer_function_list_tail = s_p_ecos_timer_function_list_head;
        free(p_timer);
    }
    else if (p_timer == s_p_ecos_timer_function_list_head)
    {
        s_p_ecos_timer_function_list_head       = p_timer->next;
        s_p_ecos_timer_function_list_head->prev = NULL;
        free(p_timer);
    }
    else if (p_timer == s_p_ecos_timer_function_list_tail)
    {
        s_p_ecos_timer_function_list_tail       = p_timer->prev;
        s_p_ecos_timer_function_list_tail->next = NULL;
        free(p_timer);
    }
    else
    {
        p_timer->prev->next = p_timer->next;
        p_timer->next->prev = p_timer->prev;
        free(p_timer);
    }

    return TRUE;
}

unsigned int ecos_timer_function_set(ecos_timer_handle hnd, ecos_fn_timer_function func, void *p_args, int interval, int count)
{
    ecos_timer_function_list_t *p_timer = NULL;

    //
    // return FALSE when handle not in list ecos_timer_function_list_t.
    //
    if ((p_timer=get_timerlist_ptr_by_handle(hnd)) == NULL)
    {
        printf("can not find handle: 0x%08X\n", hnd);
        return FALSE;
    }

    //
    // config timer ptr.
    //
    p_timer->func      = func;
    p_timer->p_args    = p_args;
    p_timer->interval  = interval;
    p_timer->count     = count;
    p_timer->last_time = ecos_get_current_uptime();

    return TRUE;
}

// ################################################################################
// suspend / resume
// ################################################################################

unsigned int ecos_timer_function_suspend(ecos_timer_handle hnd)
{
    ecos_timer_function_list_t *p_timer = NULL;

    //
    // return FALSE when handle not in list ecos_timer_function_list_t.
    //
    if ((p_timer=get_timerlist_ptr_by_handle(hnd)) == NULL)
    {
        printf("can not find handle: 0x%08X", hnd);
        return FALSE;
    }

    //
    // enable suspend, timer function will not run.
    //
    p_timer->suspend = TRUE;
    
    return TRUE;
}

unsigned int ecos_timer_function_resume(ecos_timer_handle hnd)
{
    ecos_timer_function_list_t *p_timer = NULL;

    //
    // return FALSE when handle not in list ecos_timer_function_list_t.
    //
    if ((p_timer=get_timerlist_ptr_by_handle(hnd)) == NULL)
    {
        printf("can not find handle: 0x%08X\n", hnd);
        return FALSE;
    }

    //
    // disable suspend, timer function run normal.
    //
    p_timer->suspend = FALSE;
    p_timer->last_time = ecos_get_current_uptime();
    
    return TRUE;
}

// ################################################################################
// init / free
// ################################################################################

void ecos_timer_function_free(void)
{
    //
    // del all timer in list ecos_timer_function_list_t.
    //
    while (s_p_ecos_timer_function_list_tail != NULL)
    {
        ecos_timer_function_del(s_p_ecos_timer_function_list_tail->hnd);
    }

    s_p_ecos_timer_function_list_head = NULL;
    s_p_ecos_timer_function_list_tail = s_p_ecos_timer_function_list_head;
}

void ecos_timer_function_init(void)
{
    //
    // set head & tail to nil.
    //
    s_p_ecos_timer_function_list_head = NULL;
    s_p_ecos_timer_function_list_tail = s_p_ecos_timer_function_list_head;
}

