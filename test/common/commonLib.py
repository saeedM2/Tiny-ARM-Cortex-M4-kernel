#!/usr/bin/env python

import os
import sys
import subprocess
import logging
import traceback
import glob
import json 
import paramiko
import pysftp
import shutil

from undecorated import undecorated
from functools import wraps

def configLogger(name="test"):
    if os.path.exists("logs"):
      shutil.rmtree("logs")

    os.mkdir("logs")
  
    format = logging.Formatter(fmt="[%(asctime)s] "
                                   "%(levelname)-s - "
                                   "%(message)s",
                                  datefmt="%Y-%m-%d %H:%M:%S")

    handler = logging.FileHandler("logs/scriptLog", mode="w")
    handler.setFormatter(format)
    screen_handler = logging.StreamHandler(stream=sys.stdout)
    screen_handler.setFormatter(format)
    logger = logging.getLogger(name)
    logger.setLevel(logging.DEBUG)
    
    # Print to file
    logger.addHandler(handler)
    # Print to console 
    logger.addHandler(screen_handler)

    return logger

def init(logger):
    try:
        output = exec_command("rm logs/buildLog")
        if "" not in output:
            msg = ("ERROR: exec_command() -"
                   " build all faild.\n\nOutput msg:\n{}")
            raise AssertionError(msg.format(output))
    except:
        logger.info(traceback.print_exc())

def timeit(f):
    @wraps(f)
    def timed(*args, **kw):
        args_name = inspect.getargspec(f)[0]
        args_dict = dict(itertools.izip(args_name, args))
        fh = args_dict.get("fh")
        fname = f.__name__

        ts = time.time()
        result = f(*args, **kw)
        te = time.time()
        delta = te - ts

        msg = "Time it takes to complete {}: {}\n"
        fh.write(msg.format(fname, delta))

        return result
    return timed

@timeit
def subProcessWrapper(commandString, fh, shellOffMode):
    if shellOffMode == False:
        process = subprocess.Popen(commandString,
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE,
                                   shell=True)
    else:
        process = subprocess.Popen(shlex.split(commandString),
                                   stdin=subprocess.PIPE,
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE,
                                   shell=False)
    out, err = process.communicate()
    return out, err

def exec_command(commandString,
                 logCriticalEnable = True,
                 logAllEnable = False,
                 logTimeEnable = False):
    fh = None
    if logAllEnable:
        logging.info("{}".format(commandString))

    if logTimeEnable == False:
        out, err = undecorated(subProcessWrapper)(commandString, fh, False)
    else:
        out, err = subProcessWrapper(commandString, fh, False)

    # Combine std out and std err
    commandData = out + err

    if logAllEnable:
        logging.info("{}".format(commandData))
    elif logCriticalEnable:
        # TODO: currently FW does not output these messages. Make FW do this:
        if ("Abort" in commandData) or ("ABORT_REQ" in commandData):
           logging.info("{}".format(commandString))
           logging.info("{}".format(commandData))

    # TODO: currently FW does not output these messages. Make FW do this:
    if "Interrupted system call" in str(commandData):
        raise Exception("NVME Host Command Timeout")
    return commandData

# TODO: Remove this method if unused
def flashBoard():
    gdbFile = "gdbCommands"
    # Need to test current dir of gdbFile to ensure it executes the command
    exec_command("arm-none-eabi-gdb -q -x {}".format(gdbFile))

def isStUtilRunning():
    out = exec_command("ps -aux | grep st-util").split('\n')
    proc = out[0]
    if "grep" not in proc:
        return True
    else:
        return False

def findDevice():
    serialDevice = glob.glob("/dev/ttyUSB*")[0]
    jdata = {}
    print serialDevice
    jdata = {"device":serialDevice}
    print jdata
    with open("config.json", "w") as jfile:
      json.dump(jdata, jfile)

def sshOpen(host="192.168.1.74", usr="pi", passw="pi"):
  client = paramiko.SSHClient()
  client.load_system_host_keys()
  client.connect(hostname=host, username=usr, password=passw)
  return client

def sshClose(ssh):
  ssh.close()

def exec_command_remote(cmd, ssh):
  stdin, stdout, stderr = ssh.exec_command(cmd)
  out = stdout.read()
  err = stderr.read()
  return (err + out)

def exec_command_remote_background(cmd, ssh):
  transport = ssh.get_transport()
  channel = transport.open_session()
  channel.exec_command(cmd)
  return True

def findDeviceBySerial():
  serialNum = "A50285BI"
  ssh = sshOpen()
  out = exec_command_remote("ls -l /dev/serial/by-id | grep {}".format(serialNum), ssh)
  deviceIndex = out.split("ttyUSB")
  sshClose(ssh)
  return "/dev/ttyUSB{}".format(deviceIndex[-1]).rstrip()

def sftpConnect(host="192.168.1.74", usr="pi", passw="pi"):
  sftp = pysftp.Connection(host, 
                           username=usr,
                           password=passw)
  return sftp

def sftpClose(sftp):
  sftp.close()

def sftpPut(sftp, src, dst):
  sftp.put(src, dst)

def uploadFileToRemoteHostMainDir(logger, fileName, currentDir):
    sftp = sftpConnect()
    logger.info("Uploading {} to remote host...".format(fileName))
    src = currentDir + "/{}".format(fileName)
    dest = "/home/pi/{}".format(fileName)
    logger.info("\nsrc={}\ndest={}\n".format(src, dest))
    sftpPut(sftp, src, dest)
    logger.info("Done.")
    sftpClose(sftp)
