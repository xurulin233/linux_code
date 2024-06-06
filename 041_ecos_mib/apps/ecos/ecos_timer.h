/****************************** Module Header ******************************\
* Module Name: ecos_timer.h
*
* History:
*                                   add one timer api like win32 usage.
*                                   for apps what do not use multi-threading.
\***************************************************************************/

#ifndef __ECOS_TIMER_H__
#define __ECOS_TIMER_H__

/****************************************************************************
 *
 * DESCRIPTION:
 *	typedef timer struct and handle.
 *
 ****************************************************************************/

typedef int ecos_timer_handle;
typedef void (*ecos_fn_timer_function)(void* p_args);

/****************************************************************************
 * FUNCTION:
 *	ecos_get_current_uptime
 *
 * DESCRIPTION:
 *	get system uptime (second).
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  uptime second.
 *
 ****************************************************************************/

long ecos_get_current_uptime(void);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_loop
 *
 * DESCRIPTION:
 *	run timers what have registered, use select at the same time.
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  void.
 *
 ****************************************************************************/

void ecos_timer_function_loop(void);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_show
 *
 * DESCRIPTION:
 *	show all timer function info.
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  void.
 *
 ****************************************************************************/

void ecos_timer_function_show(void);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_add
 *
 * DESCRIPTION:
 *	add a timer handle.
 *
 * PARAMETERS:
 *  timer_desc -- timer description.
 *  func       -- timer functions.
 *  p_args     -- timer args.
 *  interval   -- timer interval running (second).
 *  count      -- timer count to run.
 *
 * RETURNS:
 *  ECOS_TIMER_FUNCTION_HANDLE_INVALID: timer register error.
 *  others: return a timer handle.
 *
 ****************************************************************************/

#define ECOS_TIMER_FUNCTION_HANDLE_INVALID    (~0)
#define ECOS_TIMER_FUNCTION_COUNT_FOREVER     (~0)

ecos_timer_handle ecos_timer_function_add(char *timer_desc, ecos_fn_timer_function func, void *p_args, int interval, int count);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_del
 *
 * DESCRIPTION:
 *	del a timer handle.
 *
 * PARAMETERS:
 *  hnd -- timer handle what has registered.
 *
 * RETURNS:
 *  TRUE : successed.
 *  FALSE: failed.
 *
 ****************************************************************************/

unsigned int ecos_timer_function_del(ecos_timer_handle hnd);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_set
 *
 * DESCRIPTION:
 *	set a registered timer handle.
 *
 * PARAMETERS:
 *  hnd      -- timer handle what has registered.
 *  func     -- new timer function.
 *  p_args   -- new timer p_args.
 *  interval -- new timer interval.
 *  count    -- new timer count.
 *
 * RETURNS:
 *  TRUE : successed.
 *  FALSE: failed.
 *
 ****************************************************************************/

unsigned int ecos_timer_function_set(ecos_timer_handle hnd, ecos_fn_timer_function func, void *p_args, int interval, int count);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_suspend
 *
 * DESCRIPTION:
 *	suspend a registered timer handle.
 *
 * PARAMETERS:
 *  hnd      -- timer handle what has registered.
 *
 * RETURNS:
 *  TRUE : successed.
 *  FALSE: failed.
 *
 ****************************************************************************/

unsigned int ecos_timer_function_suspend(ecos_timer_handle hnd);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_resume
 *
 * DESCRIPTION:
 *	resume a registered timer handle.
 *
 * PARAMETERS:
 *  hnd      -- timer handle what has registered.
 *
 * RETURNS:
 *  TRUE : successed.
 *  FALSE: failed.
 *
 ****************************************************************************/

unsigned int ecos_timer_function_resume(ecos_timer_handle hnd);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_free
 *
 * DESCRIPTION:
 *	free timer function api.
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  void.
 *
 ****************************************************************************/

void ecos_timer_function_free(void);

/****************************************************************************
 * FUNCTION:
 *	ecos_timer_function_init
 *
 * DESCRIPTION:
 *	init timer function api.
 *
 * PARAMETERS:
 *
 * RETURNS:
 *  void.
 *
 ****************************************************************************/

void ecos_timer_function_init(void);

// ################################################################################

#endif// __ECOS_TIMER_H__

