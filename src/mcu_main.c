#include "mcu_api.h"
#include "mcu_errno.h"
#include <string.h>
#include "cmd_buffer.h"

void play_ir_code(struct cmd_buffer *cmd);

#define MAX_BUF 255
unsigned char buf[MAX_BUF];
struct cmd_buffer cmd;

void mcu_main() {
    debug_print(DBG_DEBUG, "mcu starting...\n");
	while (1) {
		unsigned int len;
		len = host_receive(buf, MAX_BUF);
		if (len > 4) {
    		debug_print(DBG_DEBUG, "mcu got buffer %d %s\n", len, buf);
		}
		if ((len >= 6) && (strncmp((char*)buf, "IRCODE", 6) == 0)) {
			parse_hexbuf(&cmd, buf + 6, buf, len - 6);
			debug_print(DBG_DEBUG, "playing buf %d\n", cmd.len);
			if (cmd.len > 0) {
				play_ir_code(&cmd);
			}
		}
	}
}
