/****************************** Module Header ******************************\
* File Name: tiemr_task.c
*
* Copyright (c) 2020 - 2030, ZCom Inc.
*
* timer for linux5.4
*
* History:
* 2024/03/29 xucheng@zcom.com.cn - fuck it.
\***************************************************************************/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>

static void kernel_timer_expires(struct timer_list *t);

static struct timer_list test_timer1;
//初始化timer2
static DEFINE_TIMER(test_timer2, kernel_timer_expires);

static void kernel_timer_expires(struct timer_list *t)
{
	if(t == &test_timer1){
		printk("Timer 1 is coming\r\n");
		mod_timer(&test_timer1, jiffies + 1*HZ); // 重新设置timer1 1s timeout
	}else if(t == &test_timer2){
		printk("Timer 2 is coming\r\n");
		mod_timer(&test_timer2, jiffies + 2*HZ); // 重新设置timer2 2s timeout
	}else{
		printk("Err: timer is unknown!!!\r\n");
	}
}

static int __init kernel_timer_init(void)
{
	printk("Enter timer test...\r\n");

	//初始化timer1
	timer_setup(&test_timer1, kernel_timer_expires, 0);

	//启动timer1、timer2
	mod_timer(&test_timer1, jiffies + 1*HZ); // 1s timeout
	mod_timer(&test_timer2, jiffies + 2*HZ); // 2s timeout
	
	return 0;
}

static void __exit kernel_timer_exit(void)
{	
	printk("Exit timer test!!!\r\n");
	
	del_timer(&test_timer1);	
	del_timer(&test_timer2);
}

module_init(kernel_timer_init);
module_exit(kernel_timer_exit);

