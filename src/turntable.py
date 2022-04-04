import serial
import time
import struct

class Turntable(object):

    def __init__(self, port="/dev/ttyUSB0"):
        self.serial = serial.Serial(port, 9600, timeout=1)
        time.sleep(1)
        self.serial.readall()

    def yaw_to(self, angle, block=True):
        steps = int(angle*16/1.8 * 6)
        packet = struct.pack("<ch", b"y", steps)
        self.serial.write(packet)
        if block:
            self.wait_on_command()

    def home(self, block=True):
        packet = struct.pack("<ch", b"h", 0)
        self.serial.write(packet)
        if block:
            self.wait_on_command()

    def wait_on_command(self,duration=120):
        start = time.time()
        while (self.serial.read() != b'd') and (time.time() - start) < duration:
            time.sleep(0.01)