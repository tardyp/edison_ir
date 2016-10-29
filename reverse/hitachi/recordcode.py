from sys import byteorder
from array import array
from struct import pack

import pyaudio

THRESHOLD = 5000
CHUNK_SIZE = 1024
FORMAT = pyaudio.paInt16
RATE = 44100
LONG_THRESHOLD = 80
SHORT_THRESHOLD = 30
NOISE_THRESHOLD = 2

def detect_code(r):
    code = []

    neg = 0
    pos = 0
    last_pos = 0
    o = 0
    l = 0
    start_code = 0
    for a in r:
        if a > THRESHOLD:
            pos +=1
            if start_code and neg:
                if last_pos > SHORT_THRESHOLD:
                    # startcode
                    pass
                elif neg > SHORT_THRESHOLD:
                    o |= 1 << l
                    l += 1
                elif neg > NOISE_THRESHOLD:
                    l += 1
                if l==8:
                    code.append("%02X" % o)
                    o = 0
                    l = 0
            neg = 0
        else:
            neg +=1
            if pos > 1000:
                start_code = 1
            if pos >0:
                last_pos = pos
            pos = 0
            if neg > THRESHOLD and start_code:
                s = 0xc2
                for x in code:
                   x = eval("0x" + x)
                   s = (s + x) & 0xff
                if s == 0:
                    print " ".join(code)
                else:
                    print " ".join(code), "bad checksum", s
                return 1

    if not pos and not start_code:
        r[:] = array('h')

def record():
    p = pyaudio.PyAudio()
    stream = p.open(format=FORMAT, channels=2, rate=RATE,
        input=True, output=False,
        frames_per_buffer=CHUNK_SIZE)

    num_silent = 0
    snd_started = False

    r = array('h')

    while 1:
        # little endian, signed short
        snd_data = array('h', stream.read(CHUNK_SIZE))[::2]
        if byteorder == 'big':
            snd_data.byteswap()
        r.extend(snd_data)
        if detect_code(r):
            r = array('h')

if __name__ == '__main__':
    record()
