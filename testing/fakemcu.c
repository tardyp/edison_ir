#include "mcu_api.h"
#include "mcu_errno.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <string.h>

void mcu_main(void);

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
    return 0;
}

unsigned long time_us()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}
int pwm_configure(unsigned char port, int duty_ns, int period_ns)
{
    return 0;
}
int pwm_enable(unsigned char port)
{
    return 0;

}
int pwm_disable(unsigned char port)
{
    return 0;

}

void debug_print(int level, const char *format, ...)
{
    va_list args;
    va_start (args, format);
    vfprintf (stdout, format, args);
    va_end (args);
}

int host_receive(unsigned char *buf, int length)
{
    char *str = fgets((char*)buf, length, stdin);
    if (str == NULL)
        exit(0);
    int len = strnlen((char*)buf, length);
    if (buf[len-1] == '\n')
        len--;
    return len;
}

int main(void){
    mcu_main();
}
