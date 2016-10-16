#include "mcu_api.h"
#include "mcu_errno.h"
#include "cmd_buffer.h"
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
void play_ir_code(struct cmd_buffer *cmd) {
	int i;
	int bit;

    if (cmd->len < 16) {
        debug_print(DBG_DEBUG, "bad argument len\n");
        return;
    }
    debug_print(DBG_DEBUG, "reading args\n");

    int PULSE_FREQ = cmd_read_u16(cmd);
    debug_print(DBG_DEBUG, "reading args\n");

    int START_PULSE = cmd_read_u16(cmd);
    int START_PAUSE = cmd_read_u16(cmd);
    debug_print(DBG_DEBUG, "reading args\n");
    int START_PULSE2 = cmd_read_u16(cmd);
    int START_PAUSE2 = cmd_read_u16(cmd);
    debug_print(DBG_DEBUG, "reading args\n");

    int PULSE_LEN = cmd_read_u16(cmd);
    int PAUSE_HIGH = cmd_read_u16(cmd);
    int PAUSE_LOW = cmd_read_u16(cmd);
    debug_print(DBG_DEBUG, "reading args\n");
    if (PULSE_FREQ == 0){
        debug_print(DBG_DEBUG, "PULSE_FREQ is 0!\n");
        return;
    }

    int period = 1000000/PULSE_FREQ;
    period *= 1000; // configure is in ns

    debug_print(DBG_DEBUG, "configure %d \n", period);
    pwm_configure(PORT, period/2, period);
	enable(0);
	debug_print(DBG_DEBUG, "mcu playing ir code of len %d\n", cmd->len);
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
    for(i=0; i < cmd->len; i++){
        unsigned char byte = cmd->buf[i];
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
