#include "mcu_api.h"
#include "mcu_errno.h"
#include <stdlib.h>
#include <stdio.h>

void gpio_write(int gpio, int val)
{
    printf("gpio_write %d %d\n", gpio, val);
}
void gpio_setup(int gpio, int direction)
{
    printf("gpio_setup %d %d\n", gpio, direction);
}

void mcu_sleep(int ticks)
{

}

int mcu_delay(int delay_us)
{

}

unsigned long time_us()
{

}
int pwm_configure(unsigned char port, int duty_ns, int period_ns)
{

}
int pwm_enable(unsigned char port)
{

}
int pwm_disable(unsigned char port)
{

}

int host_receive(unsigned char *buf, int length)
{
    char *str = fgets(buf, length, stdin);
    if (str == NULL)
        exit(0);
    int len = strnlen(buf, length);
    if (buf[len-1] == '\n')
        len--;
    return len;
}

void main(void){
    mcu_main();
}
