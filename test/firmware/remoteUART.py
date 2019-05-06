import os
import serial
import traceback
import json
import commands

def isStUtilRunning():
    out = commands.getstatusoutput("ps -aux | grep st-util")
    proc = str(out[1]).split("\n")
    if any("grep" not in line for line in proc):
        return True
    else:
        return False

def readUART_remote():
    fh = open("fwLog", "w")
     
    print "Reading device mount point..."
    with open("config.json", "r") as jfile:
      jdata = json.load(jfile)
    serialDevice = jdata["device"]

    if not isStUtilRunning():
        print "Starting st-util...\n"
        out = command.getstatusoutput("(st-util > stLog 2>&1)&")

    print "Reading UART port..."
    ser = serial.Serial(serialDevice, 9600)

    while True:
        sleep(0.5)
        data = ser.readline().rstrip()[2:].lstrip()
        if data:
            fh.write(data + "\n")
            fh.write("Serial output - " + data + "\n")

def main():
    try:
        readUART_remote()
    except:
        traceback.print_exc()
    finally:
        fh.close()

if __name__ == "__main__":
   main()
