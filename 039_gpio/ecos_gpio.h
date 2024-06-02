#ifndef __S5PV210_GPIO_H__
#define __S5PV210_GPIO_H__

#include <mach/gpio.h>

/****************************************************************************
 * DESCRIPTION:
 *  button status.
 ****************************************************************************/
#define GPIO_LOW               0
#define GPIO_HIGH              1

#define BTN_PRESSED		GPIO_LOW
#define BTN_UNPRESSED	GPIO_HIGH

/****************************************************************************
 * DESCRIPTION:
 *  define button gpio num.
 ****************************************************************************/
#define KP_COL0			S5PV210_GPH2(0)
#define KP_COL1			S5PV210_GPH2(1)
#define KP_COL2			S5PV210_GPH2(2)
#define KP_COL3			S5PV210_GPH2(3)


#endif // __S5PV210_GPIO_H__
