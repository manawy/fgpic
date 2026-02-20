import serial
import time

with open("data_output.csv", "a") as ofile:
    with serial.Serial('/dev/ttyACM0', 115200, timeout=10) as ser:
        while True:
            line = ser.readline()
            if not line:
                continue
            dat = line.split(b" ")
            print(dat[0], dat[1])
            ofile.write("{0},{1},{2}\n".format(
                time.time(),
                dat[0].decode("utf-8"),
                dat[1].decode("utf-8")
                ))
            ofile.flush()
