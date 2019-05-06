#!/usr/bin/env python

import sys
import os
import traceback
import serial
import json
import shutil
import time

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

def isUARTSessionRunning(logger, device):
  out = commonLib.exec_command("ps -ef | grep {}".format(device))
  logger.info(out)
  if "grep" not in out:
    return True
  else:
    return False

def isUARTSessionRunning_remote(logger, device, ssh):
  logger.info("Checking for any active remote UART sessions...\n")
  out = commonLib.exec_command_remote("ps -ef | grep {}".format(device), ssh)
  logger.info("\noutput:\n{}".format(out))
  if "grep" not in out:
    logger.info("\nActive session found\n")
    return True
  else:
    logger.info("No active session\n")
    return False

def killUART(device):
  if isUARTSessionRunning(device):
    out = commonLib.exec_command("sudo kill {}".format(device))

def killUART_remote(logger, device):
  ssh = commonLib.sshOpen()
  if isUARTSessionRunning_remote(logger, device, ssh):
    out = commonLib.exec_command_remote("sudo kill {}".format(device), ssh)
  commonLib.sshClose(ssh)

def readUART_local(logger):
    logger.info("\n-----------Printing serial console logs-----------\n")
    if not commonLib.isStUtilRunning():
      out = commonLib.exec_command("(st-util > stLog 2>&1)&")
    logger.info(os.getcwd())

    with open("config.json", "r") as jfile:
      jdata = json.load(jfile)
    serialDevice = jdata["device"]

    killUART(serialDevice)

    ser = serial.Serial(serialDevice, 9600)

    while True:
        data = ser.readline().rstrip()[2:].lstrip()
        if data:
            logger.info(data)
            logger.info("Serial output - " + data)

def readUART_remote(logger, ssh, ftp_h):
    logger.info("\n-----------Printing serial console logs-----------\n")
    currentDir = os.getcwd()

    logger.info("current dir = {}".format(currentDir))
    logger.info("Reading FW logs on remote host...\n")

    timeElapsed = time.time()
    sleepPeriod = 10
    timeoutPeriod = 60*60*2

    remoteFile = ftp_h.open("/home/pi/fwLog")
    while (timeoutPeriod - timeElapsed):
      for line in remoteFile:
        logger.info("FW Log entry: {}".format(line))
      logger.info("Polling logs in {}s".format(sleepPeriod))
      time.sleep(sleepPeriod)
      timeElapsed = time.time()

    killUART_remote(logger, serialDevice)
 
def main():
    try:
      logger = commonLib.configLogger()
      ssh = commonLib.sshOpen()
      ftp_h = commonLib.sftpConnect()
      readUART_remote(logger, ssh, ftp_h)
    except:
      traceback.print_exc()
    finally:
      commonLib.sshClose(ssh)
      commonLib.sftpClose(ftp_h)
if __name__ == "__main__":
   main()