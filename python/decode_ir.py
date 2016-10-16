

def decode_ir(fn, THRESHOLD=100):
    bursts = []
    t = 0
    last_one = 0
    burst_start = 0
    with open(fn) as f:
        b = f.read(1)
        while len(b) > 0:
            b = ord(b)
            if b > 0:
                if t - last_one  > THRESHOLD:
                    burst_start = t
                last_one = t
            if t - last_one > THRESHOLD and not (bursts and bursts[-1][0] == burst_start):
                bursts.append((burst_start, last_one - burst_start))

            b = f.read(1)
            t += 1
    return bursts
