#include "mcu_api.h"
#include "mcu_errno.h"
#include <string.h>

void play_ir_code(unsigned char *codes, int len);

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
int parse_hexbuf(unsigned char *src, unsigned char *dst, int len)
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
    debug_print(DBG_DEBUG, "mcu starting...\n");
	while (1) {
		unsigned int len;
		len = host_receive(buf, MAX_BUF);
		if ((len >= 6) && (strncmp((char*)buf, "IRCODE", 6) == 0)) {
			len = parse_hexbuf(buf + 6, buf, len - 6);
			if (len > 0) {
				play_ir_code(buf, len);
			}
		}
	}
}
