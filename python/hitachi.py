#!/usr/bin/env python
base_cmd = [
    0x1, 0x10, 0x30, 0x40, 0xBF, 0x1, 0xFE, 0x11, 0x12, 0x1, 0x3, 0x20,
    0x0, 0x2, 0x0, 0x3, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,
    0x0, 0x0, 0]

PULSE_FREQ = 38000

START_PULSE = 30600
START_PAUSE = 51100
START_PULSE2 = 3356
START_PAUSE2 = 1723

PULSE_LEN = 430
PAUSE_HIGH = 1247
PAUSE_LOW = 430


def make_header():
    ret = ""
    for i in [PULSE_FREQ, START_PULSE, START_PAUSE, START_PULSE2,
              START_PAUSE2, PULSE_LEN, PAUSE_HIGH, PAUSE_LOW]:
        ret += "%04X" % i
    return ret


def create_cmd(temperature, ventilation, mode, powerful, eco, on):
    buf = base_cmd[:]
    c = 0xc2
    buf[11] = temperature << 1
    buf[13] = ventilation + 1
    buf[10] = mode
    if (powerful):
        buf[25] = 0x20
    if (eco):
        buf[25] = 0x02
    if (not on):
        buf[19] = 0x0

    for i in xrange(27):
        c = (c + buf[i]) & 0xff

    c = 0xff ^ (c - 1)
    buf[27] = c
    return make_header() + "".join(["%02X" % i for i in buf])


def run_cmd(temperature, ventilation, mode, powerful, eco, on):
    # we import here to be able to run on host
    import mraa
    buf = create_cmd(temperature, ventilation, mode, powerful, eco, on)
    # mraa will make sure the Pwm is configured properly
    x = mraa.Pwm(3)
    x.period_us(10)
    x.pulsewidth_us(5)
    x.enable(True)
    with open("/dev/ttymcu0", "w") as f:
        f.write("IRCODE" + buf + "\n")
    x.enable(False)


def mode(s):
    return {"HOT": 3, "COLD": 4, "HUM": 5}[s]


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='Create a hitachi cmd')
    parser.add_argument('--temperature', type=int, default=22,
                        help='target temperature')
    parser.add_argument('--ventilation', type=int, default=2,
                        help='target ventilation')
    parser.add_argument('--mode', type=mode, default=3,
                        help='target mode')
    parser.add_argument('--powerful', action="store_true", default=False,
                        help='powerful mode')
    parser.add_argument('--eco', action="store_true", default=False,
                        help='eco mode')
    parser.add_argument('--on', action="store_true", default=False,
                        help='enable')
    args = parser.parse_args()
    run_cmd(args.temperature, args.ventilation, args.mode, args.powerful, args.eco, args.on)
