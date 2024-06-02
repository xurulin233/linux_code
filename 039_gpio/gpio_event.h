#ifndef __GPIO_EVENT_H__
#define __GPIO_EVENT_H__

#define GPIO_IOCTL_DEV_MAJOR         215
#define GPIO_IOCTL_DEV_NAME          "/dev/gpio"

#define GPIO_IO_BASE                 0x7100
#define GPIO_GET_LED                 (GPIO_IO_BASE + 1)
#define GPIO_GET_BTN                 (GPIO_IO_BASE + 2)
#define GPIO_SET_LED                 (GPIO_IO_BASE + 3)
#define GPIO_SET_LED_BLINK           (GPIO_IO_BASE + 4)


#define NETLINK_GPIO_EVENT                  (26)        // gpio event netlink id.
#define BTN_PRESSE_2SEC                     (2  * HZ)   // button has pressed 2 second then unpressed.
#define BTN_PRESSE_3SEC                     (3  * HZ)   // button has pressed 3 second then unpressed.
#define BTN_PRESSE_30SEC                    (30 * HZ)   // button has pressed 30 second then unpressed.
#define BTN_PRESSE_REBOOT                   (2)         // button has pressed 2 second then unpressed.
#define BTN_PRESSE_DEFAULT                  (5)         // button has pressed 30 second then unpressed.

/****************************************************************************
 *
 * DESCRIPTION:
 *  app ioctl struct.
 *
 ****************************************************************************/

typedef struct gpio_mgmt_t
{
    unsigned int        gpio_offset;            // gpio offset.
    unsigned int        gpio_val;               // gpio val.
    unsigned int        gpio_last_val;          // gpio last val.
    unsigned int        blink_enable;           // led blink enable.
    unsigned int        blink_interval;         // led blink interval (interval is multiples of 50ms).
    unsigned int        blink_count;            // led blink count (one count means 'one on and one off').
    unsigned int        blink_val;              // led blink current val.
    unsigned int        blink_cd;               // led blink cool down time.
    unsigned char       cpld_data1; 	        //cpld data1
    unsigned char       cpld_data2; 	        //cpld data2
    unsigned char       cpld_txrx; 	            //cpld tx/rx flag
} gpio_mgmt_t;


#endif // __GPIO_EVENT_H__
