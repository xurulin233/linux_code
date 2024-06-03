
#ifndef __ECOS_PRODUCT_MISC_H__
#define __ECOS_PRODUCT_MISC_H__
#include <linux/netlink.h>
#include <sys/socket.h>  
#include "ecos_type.h"

typedef struct btn_press_netlink_t
{
    struct nlmsghdr nlhdr;          // netlink header.
    unsigned int    press_time;     // netlink val.
} btn_press_netlink_t;

void ecos_product_misc_init(void);
bool_t ecos_product_dev_button_check(fd_set* fset);

extern int g_button_check_fd;

#endif // __ECOS_PRODUCT_MISC_H__
