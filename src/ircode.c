#include "mcu_api.h"
#include "mcu_errno.h"
#include <string.h>

// Arduino Extension PIN = 3
#define PORT 0


int num_pulse=0;
void enable(int value)
{
	if (value){
        pwm_enable(PORT);
		num_pulse ++;
	}
	else
        pwm_disable(PORT);
}
unsigned long start_time = 0;

/* we use active delay timing, as delay_us and co are not precise enough */
void delay(int us)
{
    if(time_us() - start_time > us) {
        debug_print(DBG_DEBUG, "missed a timing!\n");
        start_time = time_us();
    }
    while(time_us() - start_time < us);
    start_time += us;
}
int read_u16(unsigned char **buf, int *len)
{
    int ret;
    if (*len < 2)
        return -1;
    *len -= 2;
    ret = (*buf[0]<<8) | *buf[1];
    *buf += 2;
    return ret;
}
void play_ir_code(unsigned char *codes, int len) {
	int i;
	int bit;

    if (len < 16) {
        debug_print(DBG_DEBUG, "bad argument len\n");
        return;
    }
    int PULSE_FREQ = read_u16(&codes, &len);

    int START_PULSE = read_u16(&codes, &len);
    int START_PAUSE = read_u16(&codes, &len);
    int START_PULSE2 = read_u16(&codes, &len);
    int START_PAUSE2 = read_u16(&codes, &len);

    int PULSE_LEN = read_u16(&codes, &len);
    int PAUSE_HIGH = read_u16(&codes, &len);
    int PAUSE_LOW = read_u16(&codes, &len);

    int period = 1000000/PULSE_FREQ;
    period *= 1000; // configure is in ns

    pwm_configure(PORT, period/2, period);
	enable(0);
	debug_print(DBG_DEBUG, "mcu playing ir code of len %d\n", len);
	start_time = time_us();
    /* send start bit */
    enable(1);
    delay(START_PULSE);
    enable(0);
    delay(START_PAUSE);
    enable(1);
    delay(START_PULSE2);
    enable(0);
    delay(START_PAUSE2);
	start_time = time_us();
    /* send all bits of each bytes */
    for(i=0; i < len; i++){
        unsigned char byte = codes[i];
        for(bit=0; bit < 8; bit++){
            int bit_value = (byte >> bit) & 1;
            enable(1);
            delay(PULSE_LEN);
            enable(0);
            if (bit_value)
                delay(PAUSE_HIGH);
            else
                delay(PAUSE_LOW);
        }
    }
    /* send last bit */
    enable(1);
    delay(PULSE_LEN);
    enable(0);
}
