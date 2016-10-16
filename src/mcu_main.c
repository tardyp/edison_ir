#include "mcu_api.h"
#include "mcu_errno.h"
#include <string.h>

// Arduino Extension PIN = 3
#define PORT 0

const int PULSE_FREQ = 38000;

const int START_PULSE = 30600;
const int START_PAUSE = 51100;
const int START_PULSE2 = 3356;
const int START_PAUSE2 = 1723;

const int PULSE_LEN = 430;
const int PAUSE_HIGH = 1247;
const int PAUSE_LOW = 430;
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
void delay(int us)
{
    if(time_us() - start_time > us) {
        debug_print(DBG_DEBUG, "missed a timing!\n");
        start_time = time_us();
    }
    while(time_us() - start_time < us);
    start_time += us;
}
void play_ir_code(unsigned char *codes, int len) {
	int i;
	int bit;
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
int fromhex(char c){
	if (c >= 'a' && c <= 'f') {
		return 0xa + (c - 'a');
	}
	if (c >= 'A' && c <= 'F') {
		return 0xa + (c - 'A');
	}
	if (c >= '0' && c <= '9') {
		return (c - '0');
	}
	return -1;
}
int parse_ir_code(unsigned char *src, unsigned char *dst, int len)
{
	int i;
	int a, b;
	if (len % 2 == 1) {
    	debug_print(DBG_DEBUG, "mcu got buffer with bad len %d...\n", len);
		return -1;
	}
	for(i = 0; i < len; i+=2)
	{
		a = fromhex(src[i]);
		b = fromhex(src[i + 1]);
		if (a < 0 || b < 0) {
			return i/2;
		}
		dst[i/2] = (a << 4) | b;
	    debug_print(DBG_DEBUG, "%d\n", dst[i/2]);
	}
	return len/2;
}
#define MAX_BUF 255
unsigned char buf[MAX_BUF];

void mcu_main() {
    int period = 1000000/PULSE_FREQ;
    period *= 1000; // configure is in ns
    debug_print(DBG_DEBUG, "mcu starting...\n");

    debug_print(DBG_DEBUG, "mcu starting loop...\n");
	while (1) {
		unsigned int len;

		len = host_receive(buf, MAX_BUF);
		if ((len >= 6) && (strncmp((char*)buf, "IRCODE", 6) == 0)) {
	    	debug_print(DBG_DEBUG, "mcu got buf %d %s...\n", len, buf);
			len = parse_ir_code(buf + 6, buf, len - 6);
			if (len > 0) {
			    pwm_configure(PORT, period/2, period);
				enable(0);
				num_pulse = 0;
				debug_print(DBG_DEBUG, "mcu playing code len %d\n", len);
				play_ir_code(buf, len);
				debug_print(DBG_DEBUG, "played %d pulses\n", num_pulse);
			}
		}
	}
}
