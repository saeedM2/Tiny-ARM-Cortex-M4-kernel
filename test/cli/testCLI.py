#!/usr/bin/env python

import sys
import os
import traceback
import serial
import json
import shutil

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

# DEBUG: Suppressed, remove later
if 0:
  commonLib.findDevice()

def isUARTSessionRunning(device):
  out = commonLib.exec_command("ps -ef | grep {}".format(device))
  print out
  if "grep" not in out:
    return True
  else:
    return False

def isUARTSessionRunning_remote(device, ssh):
  out = commonLib.exec_command_remote("ps -ef | grep {}".format(device), ssh)
  print out
  if "grep" not in out:
    return True
  else:
    return False

def killUART(device):
  if isUARTSessionRunning(device):
    out = commonLib.exec_command("sudo kill {}".format(device))

def killUART_remote(device):
  ssh = commonLib.sshOpen()
  if isUARTSessionRunning_remote(device, ssh):
    out = commonLib.exec_command_remote("sudo kill {}".format(device), ssh)
  commonLib.sshClose(ssh)

def readUART_local(logger):
    logger.info("\n-----------Printing serial console logs-----------\n")
    if not commonLib.isStUtilRunning():
      out = commonLib.exec_command("(st-util > stLog 2>&1)&")
    print os.getcwd()

    with open("config.json", "r") as jfile:
      jdata = json.load(jfile)
    serialDevice = jdata["device"]

    killUART(serialDevice)

    ser = serial.Serial(serialDevice, 9600)

    while True:
        data = ser.readline().rstrip()[2:].lstrip()
        if data:
            logger.info(data)
            print "Serial output - " + data

def readUART_remote(logger):
    logger.info("\n-----------Printing serial console logs-----------\n")
    ssh = commonLib.sshOpen()
    print 1
    if not commonLib.isStUtilRunning_remote():
      out = commonLib.exec_command_remote("(st-util > stLog 2>&1)&", ssh)
    print os.getcwd()
    print 2
    serialDevice = commonLib.findDeviceBySerial()
    print 3
    killUART_remote(serialDevice)
    print 3.5
    command = ("sudo chmod 666 {0} && "
               "sudo chown root:root {0} && "
               "cat {0} -s 9600".format(serialDevice))
    logger.info(command)
    out = commonLib.exec_command_remote("\nOutput:\n{}".format(command), ssh)
    logger.info(out)
    print 4
    while True:
        data = ser.readline().rstrip()[2:].lstrip()
        if data:
            logger.info(data)
            print "Serial output - " + data
    commonLib.sshClose()

def main():
    try:
      logger = commonLib.configLogger()
      readUART_remote(logger)
    except:
      logger.info(traceback.print_exc())
    finally:
      ssh.close()

if __name__ == "__main__":
   main()