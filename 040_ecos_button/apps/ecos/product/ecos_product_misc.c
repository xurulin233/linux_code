#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include "ecos_product_misc.h"
#include "gpio_event.h"
#include "ecos_dev_api.h"

int g_button_check_fd = -1;


/********************************************************
 * ecos_product_misc_init.
 ********************************************************/
void ecos_product_misc_init(void)
{
    //
    // create button check low socket.
    //
    g_button_check_fd = ecos_create_kernel_socket(NETLINK_GPIO_EVENT);
    if (g_button_check_fd < 0)
    {
        printf("register reset button netlink socket failed\n");
    }

}

/********************************************************
 * check button pressed.
 ********************************************************/
bool_t ecos_product_dev_button_check(fd_set* fset)
{
    btn_press_netlink_t btn_press_time;
    struct sockaddr_nl addr;
    socklen_t addr_len;
    int rcvlen;
    
    // 
    // check fd whether to me.
    // 
    if (!FD_ISSET(g_button_check_fd, fset))
        return FALSE;
    // 
    // recieve kernel socket fd syslog data.
    // 
    addr_len = sizeof(btn_press_netlink_t);
    rcvlen = recvfrom(g_button_check_fd, &btn_press_time, sizeof(btn_press_netlink_t),
        0, (struct sockaddr*)&addr, &addr_len);

    if (BTN_PRESSE_REBOOT == btn_press_time.press_time)
    {
        printf("@@@ BTN_PRESSE_REBOOT\n");
    }
    else if (BTN_PRESSE_DEFAULT == btn_press_time.press_time)
    {
        printf("@@@ BTN_PRESSE_DEFAULT\n");
    }

    return TRUE;
}
