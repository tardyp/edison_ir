
struct cmd_buffer{
    unsigned char *buf;
    int len;
};

int cmd_read_u16(struct cmd_buffer *buf);
int parse_hexbuf(struct cmd_buffer *cmd, unsigned char *src, unsigned char *dst, int len);
