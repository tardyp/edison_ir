#include "mcu_api.h"
#include "mcu_errno.h"
#include <stdlib.h>

void gpio_write(int gpio, int val)
{

}
void gpio_setup(int gpio, int direction)
{

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

int host_receive(unsigned char *buf, int length)
{
    exit(0);
}

void main(void){
    mcu_main();
}
