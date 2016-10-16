#include "cmd_buffer.h"
#include "mcu_api.h"

int cmd_read_u16(struct cmd_buffer *buf)
{
    int ret;
    if (buf->len < 2)
        return -1;
    buf->len -= 2;
    ret = (buf->buf[0]<<8) | buf->buf[1];
    buf->buf += 2;
    return ret;
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

int parse_hexbuf(struct cmd_buffer *cmd, unsigned char *src, unsigned char *dst, int len)
{
	int i;
	int a, b;
    cmd->buf = dst;
	if (len % 2 == 1) {
    	debug_print(DBG_DEBUG, "mcu got buffer with bad len %d...\n", len);
		return -1;
	}
	for(i = 0; i < len; i+=2)
	{
		a = fromhex(src[i]);
		b = fromhex(src[i + 1]);
		if (a < 0 || b < 0) {
            cmd->len = i/2;
            return 0;
		}
		dst[i/2] = (a << 4) | b;
	}
    cmd->len = len/2;
    return 0;
}
