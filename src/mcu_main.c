#include "mcu_api.h"
#include "mcu_errno.h"

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

void enable(int value)
{
	if (value)
        pwm_enable(PORT);
	else
        pwm_disable(PORT);
}
int play_ir_code(char *codes, int len) {
	mcu_delay(1);
	unsigned long t0 = time_us();

    /* send start bit */
    enable(1);
    mcu_delay(START_PULSE);
    enable(0);
    mcu_delay(START_PAUSE);
    enable(1);
    mcu_delay(START_PULSE2);
    enable(0);
    mcu_delay(START_PAUSE2);
    /* send all bits of each bytes */
    for(int i=0; i < len; i++){
        unsigned char byte = codes[i];
        for(int bit=0; bit < 8; bit++){
            int bit_value = (byte >> bit) & 1;
            enable(1);
            mcu_delay(PULSE_LEN);
            enable(0);
            if (bit_value)
                mcu_delay(PAUSE_HIGH);
            else
                mcu_delay(PAUSE_LOW);
        }
    }
    /* send last bit */
    enable(1);
    mcu_delay(PULSE_LEN);
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
int parse_ir_code(char *src, char *dst, int len)
{
	int i;
	int a, b;
	if (len % 2 == 1)
		return -1;

	for(i = 0; i < len; i+=2)
	{
		a = fromhex(src[i]);
		b = fromhex(src[i + 1]);
		if (a < 0 || b < 0)
			return -1;
		dst[i/2] = (a << 4) | b;
	}
	return 0;
}
#define MAX_BUF 255
unsigned char buf[MAX_BUF];

void mcu_main() {
    int period = 1000000/PULSE_FREQ;

    pwm_configure(PORT, period/2, period);
	enable(0);
	while (1) {
		unsigned int len;
		len = host_receive(buf, MAX_BUF);

		if ((len >= 5) && (strncmp(buf, "IRCODE", 6) == 0)) {
			if (parse_ir_code(buf + 6, buf, len - 6) == 0)
				play_ir_code(buf, (len - 5)/2);
		}
	}
}
