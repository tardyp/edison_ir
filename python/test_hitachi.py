import os

import decode_ir
import hitachi

cmd = hitachi.create_cmd(24, 1, 3, 0, 0, 1)
os.system("echo IRCODE%s | ./test" % (cmd))

bursts = decode_ir.decode_ir("pwm.bin")

start_1 = bursts[0]
start_2 = bursts[1]
codes = bursts[2:]

assert start_1[1] > 30000 and start_1[1] < 40000
assert start_2[1] > 3000 and start_2[1] < 4000
assert len(codes) / 8.0 == 28.0
