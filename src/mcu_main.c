#include "mcu_api.h"
#include "mcu_errno.h"

// Arduino Extension PIN = 3
#define TRIG 12

int play_ir_code(char *codes, int len) {
	// Send Trig signal to HC-SR04
	gpio_write(TRIG, 1);
	mcu_delay(10);
	gpio_write(TRIG, 0);
	mcu_delay(1);
	unsigned long t0 = time_us();

}

#define MAX_BUF 255
unsigned char buf[MAX_BUF];

void mcu_main() {
	// Setup Trig as OUTPUT
	gpio_setup(TRIG, 1);
	// Initially set Trig to LOW
	gpio_write(TRIG, 0);

	while (1) {
		unsigned int len;
		len = host_receive(buf, MAX_BUF);

		if ((len >= 5) && (strncmp(buf, "IRCODE", 5) == 0)) {
			play_ir_code(buf + 5, len - 5);
		}
	}
}
