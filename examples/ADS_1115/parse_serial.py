import serial
import time

with open("data_output.csv", "a") as ofile:
    with serial.Serial('/dev/ttyACM0', 115200, timeout=10) as ser:
        while True:
            line = ser.readline()
            if not line:
                continue
            ofile.write("{0}\t{1}".format(time.time(), line.decode("utf-8")))

            ofile.flush()
