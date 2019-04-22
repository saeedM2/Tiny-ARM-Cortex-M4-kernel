#!/usr/bin/env python

import sys
import os
import traceback
import serial
import json
import shutil

import binascii
import bitstring

if not os.path.exists("logs/"):
    os.makedirs("logs/")
else:
    shutil.rmtree("logs/")
    os.makedirs("logs/")

sys.path.append(os.getcwd())
sys.path.append(os.path.abspath("../common"))

import commonLib

path = commonLib.exec_command("pwd")
path = path.strip("\n")

commonLib.findDevice()

def isUARTSessionRunning(device):
  out = commonLib.exec_command("ps -ef | grep {}".format(device))
  print out
  if "grep" not in out:
    return True
  else:
    return False

def killUART(device):
  if isUARTSessionRunning(device):
    out = commonLib.exec_command("sudo kill {}".format(device))

def readUART(logger):
    logger.info("\n-----------Printing serial console logs-----------\n")
    if not commonLib.isStUtilRunning():
      out = commonLib.exec_command("(st-util > stLog 2>&1)&")
    print os.getcwd()

    with open("config.json", "r") as jfile:
      jdata = json.load(jfile)
    serialDevice = jdata["device"]

    killUART(serialDevice)

    out = commonLib.exec_command("sudo chmod 666 {0} "
                                 "&& sudo chown root:root {0}".format(serialDevice))
    ser = serial.Serial(serialDevice, 9600)

    while True:
        data = ser.readline().rstrip()[2:].lstrip()
        if data:
            logger.info(data)
            print "Serial output - " + data

def main():
    try:
      logger = commonLib.configLogger()
      readUART(logger)
    except:
      logger.info(traceback.print_exc())

if __name__ == "__main__":
   main()