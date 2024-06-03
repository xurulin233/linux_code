#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/netlink.h>
#include <linux/net.h>
#include <net/sock.h>
#include <mach/gpio.h>
#include "ecos_gpio.h"
#include "gpio_event.h"

#define BTN_CHECK_TIME          (50)

static gpio_mgmt_t gpio_status[] = 
{
    { KP_COL0,		BTN_UNPRESSED,	BTN_UNPRESSED,	0,	0 },
};


static struct sock *s_sk_gpio_event;

static struct task_struct *s_btn_check_task = NULL;

/****************************************************************************
 *  netlink function.
 ****************************************************************************/
static struct sock *s_sk_gpio_event;
static int send_btn_press_signal(unsigned int press_time)
{
    int size;
    unsigned char   *old_tail = NULL;
    struct sk_buff  *skb      = NULL;
    struct nlmsghdr *nlh      = NULL;
    unsigned int    *data     = NULL;
		
    // 
    // alloc new skb for netlink broadcast.
    // 
    size = NLMSG_SPACE(sizeof(unsigned int));
    if (NULL == (skb = alloc_skb(size, GFP_KERNEL)))
        goto nlmsg_failure;
    NETLINK_CB(skb).pid = 0;
    NETLINK_CB(skb).dst_group = 1;

    // 
    // fill skb header, NLMSG_PUT will set skb offset.
    // 
	
    old_tail = skb->tail;
    nlh = NLMSG_PUT(skb, 0, 0, NLMSG_DONE, size - sizeof(struct nlmsghdr));
    nlh->nlmsg_len = skb->tail - old_tail;
    nlh->nlmsg_pid = 0;
    nlh->nlmsg_flags = 0;

    // 
    // set netlink skb data content.
    // 
    data = NLMSG_DATA(nlh);
    *(unsigned int *)data = press_time;

    // 
    // netlink_broadcast skb from s_sk_gpio_event to apps.
    // 
    return netlink_broadcast(s_sk_gpio_event, skb, 0, 1, GFP_KERNEL);

nlmsg_failure: /* for NLMSG_PUT cause error. */
    if (skb) kfree_skb(skb);
    return -ENOBUFS;
}







static int btn_check_func(void* arg)
{
#define BTN_TASK_START  do {
#define BTN_TASK_END    } while(!kthread_should_stop());

    static unsigned int s_btn_last_status = BTN_UNPRESSED;
    static unsigned int s_btn_now_status  = BTN_UNPRESSED;
    static unsigned long s_btn_start_press_time = 0;

    BTN_TASK_START


    s_btn_now_status = gpio_get_value(KP_COL0);

    //
    // when button pressed time fit, blink led to prompt.
    //
    if (BTN_PRESSED == s_btn_now_status)
    {
        if (s_btn_last_status != BTN_PRESSED)
            s_btn_start_press_time = jiffies;

    }
    //
    // when button unpressed time fit, send reboot or default signal to apps.
    //
    else
    {
        if (s_btn_last_status == BTN_PRESSED)
        {
				printk("@@@ BTN_UNPRESSED jiffies = %ld s_btn_start_press_time = %ld %ld S  HZ = %d \n"
				,jiffies,s_btn_start_press_time,(jiffies - s_btn_start_press_time)/HZ,HZ);
                send_btn_press_signal(BTN_PRESSE_DEFAULT);
        }
        s_btn_start_press_time = 0;
    }

    s_btn_last_status = s_btn_now_status;

    schedule_timeout_interruptible(HZ/BTN_CHECK_TIME); // min check time (ms).

    BTN_TASK_END

    return 0;
}



/****************************************************************************
 *  ioctl for apps.
 ****************************************************************************/

static int get_gpio_index(unsigned int gpio_offset)
{
    int index = 0;

    for (; index<sizeof(gpio_status)/sizeof(gpio_status[0]); index++)
    {
        if (gpio_offset == gpio_status[index].gpio_offset)
            return index;
    }

    return -1;
}



static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

    gpio_mgmt_t gpio_st_in;
    unsigned int index;

    //
    // copy data from user.
    //
    if(copy_from_user(&gpio_st_in, (void *)arg, sizeof(gpio_st_in)) != 0) {
		return -EFAULT;
	}
    index = get_gpio_index(gpio_st_in.gpio_offset);
	
	switch (cmd)
    {
    case GPIO_GET_BTN:
        gpio_st_in.gpio_val = gpio_get_value(gpio_st_in.gpio_offset);
        if(copy_to_user((void*)arg, (void *)&gpio_st_in, sizeof(gpio_st_in))!= 0) {
			return -EFAULT;
		}
        return 0;
	}
    return -EFAULT;
}


static const struct file_operations s_gpio_ioctl_fops = 
{
    .owner  = THIS_MODULE,
	.unlocked_ioctl  = gpio_ioctl,
};


static int __init gpio_init(void)
{

	gpio_request(KP_COL0,"KP_COL0 BUTTON");
	gpio_direction_input(KP_COL0);

	//
	// register char dev.
	//
	register_chrdev(GPIO_IOCTL_DEV_MAJOR, GPIO_IOCTL_DEV_NAME,
	&s_gpio_ioctl_fops);

    // 
    // create netlink gpio event socket.
    // 
	s_sk_gpio_event = netlink_kernel_create(&init_net, NETLINK_GPIO_EVENT, 0,NULL, NULL, THIS_MODULE);
	if (s_sk_gpio_event == NULL)
    	{
        	printk(KERN_ALERT "gpio: failed to create netlink socket.\n");
        	unregister_chrdev(GPIO_IOCTL_DEV_MAJOR, GPIO_IOCTL_DEV_NAME);
        	return -ENOMEM;
    	}

	//
	// init button check task.
	//
	s_btn_check_task = kthread_run(btn_check_func, NULL, "btn_check_thread");
	if (0 != IS_ERR(s_btn_check_task))
        	return -EBUSY;

	printk(KERN_INFO "gpio init successed\n");

	return 0;
}


static void __exit gpio_exit(void)
{

	//
    // del button check task.
    //
    kthread_stop(s_btn_check_task);
    s_btn_check_task = NULL;


    // 
    // release netlink gpio event socket.
    // 
    sock_release(s_sk_gpio_event->sk_socket);

    //
    // unregister char dev.
    //
    unregister_chrdev(GPIO_IOCTL_DEV_MAJOR, GPIO_IOCTL_DEV_NAME);

    // gpio free
    gpio_free(KP_COL0);

   printk(KERN_INFO "gpio free successed\n");

}
module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xurulin");
