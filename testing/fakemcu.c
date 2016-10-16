#include "mcu_api.h"
#include "mcu_errno.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <string.h>

void mcu_main(void);

int pwm_duty = 0;
int pwm_period = 0;
int pwm_enabled = 0;
unsigned long pwm_cur_time = 0;
unsigned long pwm_last_period = 0;
unsigned long pwm_sample_time = 1;
char pwm_cur_sample = 0;
FILE *pwm_f = NULL;

void pwm_emulate_to(unsigned int t)
{
    if (pwm_f == NULL)
        pwm_f = fopen("pwm.bin", "wb");
    if (pwm_cur_time == 0) {
        pwm_cur_time = t;
        pwm_last_period = t;
    }

    while(pwm_cur_time < t){
        pwm_cur_time += pwm_sample_time;
        fwrite(&pwm_cur_sample, 1, 1, pwm_f);
        if (pwm_last_period + pwm_duty < pwm_cur_time)
            pwm_cur_sample = 0;
        if (pwm_last_period + pwm_period < pwm_cur_time) {
            if (pwm_enabled)
                pwm_cur_sample = 100;
            else
                pwm_cur_sample = 0;
            pwm_last_period += pwm_period;
        }
    }
}


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
    unsigned long ret = tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
    pwm_emulate_to(ret);
    return ret;
}


int pwm_configure(unsigned char port, int duty_ns, int period_ns)
{
    pwm_duty = duty_ns / 1000;
    pwm_period = period_ns / 1000;
    return 0;
}
int pwm_enable(unsigned char port)
{
    pwm_enabled = 1;
    return 0;

}
int pwm_disable(unsigned char port)
{
    pwm_enabled= 0;
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
