/****************************** Module Header ******************************\
* Module Name: btnconfig.c
\***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "../ecos_gpio.h"
#include "../gpio_event.h"

/****************************************************************************
 *  gpio target list.
 ****************************************************************************/

typedef struct gpio_device_t
{
    unsigned short              gpio_offset;
    const char                  *gpio_name;
} gpio_device_t;

static gpio_device_t gpio_device_list[] = 
{
    { KP_COL0, "btn_default" }, 
};

/****************************************************************************
 *  show help.
 ****************************************************************************/

static void show_help(char *cmd_name)
{
    int i;

    printf("usage: %s get <btn>\n", cmd_name);

    printf("btn: ");
    for (i=0; i<sizeof(gpio_device_list)/sizeof(gpio_device_list[0]); i++)
        printf("%s ", gpio_device_list[i].gpio_name);
    printf("\n");
    printf("example: %s get %s\n", cmd_name, gpio_device_list[0].gpio_name);
}

static int get_dev_offset(char *dev_name)
{
    int i;
    
    for (i=0; i<sizeof(gpio_device_list)/sizeof(gpio_device_list[0]); i++)
    {
        if (0 == strcmp(dev_name, gpio_device_list[i].gpio_name))
            return gpio_device_list[i].gpio_offset;
    }
    return ~0;
}

static int do_gpio_ioctl(int cmd, void *pdata)
{
    int fd, error;
    
    //
    // open dev GPIO_IOCTL_DEV_NAME.
    //
    fd = open(GPIO_IOCTL_DEV_NAME, O_RDWR);
	if (fd < 0)
    {
        perror("fd fail");
        return -1;
    }

    //
    // do ioctl GPIO_IOCTL_DEV_NAME.
    //
	if ((error=ioctl(fd, cmd, pdata)) < 0)
        printf("ioctl failed: %d\n", error);

    close(fd);
    return error;
}

/****************************************************************************
 *  main function.
 ****************************************************************************/

int main(int argc, char *argv[])
{
    gpio_mgmt_t gpio_st;
    
    //
    // check and get gpio dev status.
    //
    if ((argc == 3) && (0 == strcmp(argv[1], "get")))
    {
        if (~0 == (gpio_st.gpio_offset=get_dev_offset(argv[2])))
            goto err_command;

        do_gpio_ioctl(GPIO_GET_BTN, &gpio_st);
        printf("%s status: %s\n", argv[2], gpio_st.gpio_val==BTN_PRESSED ? "pressed" : "unpressed");
    }
    //
    // invalid setting of gpioconfig.
    //
    else goto err_command;

    return 0;

err_command:
    show_help(argv[0]);
    return -1;
}

