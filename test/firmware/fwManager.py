import sys
import os
import time
import traceback
import signal 
import json

from argparse import ArgumentParser

sys.path.append(os.getcwd())
sys.path.append(os.path.abspath("../common"))

import commonLib

parser = ArgumentParser()

parser.add_argument("-b", "--build", 
                    action="store_true", default=False,
                    help="Build elf firmare binary.")
parser.add_argument("-e", "--execute_local",
                    action="store_true", default=False, 
                    help="Execute binary from gdb, locally.")
parser.add_argument("-u", "--fwup",
                    action="store_true", default=False, 
                    help="Firmware upload into remote host (programmer).")
parser.add_argument("-r", "--execute_remote",
                    action="store_true", default=False, 
                    help="From remote host, execute binary using gdb.")
parser.add_argument("-k", "--killrfw",
                    action="store_true", default=False, 
                    help="From remote host, manually kill running FW.")
parser.add_argument("-p", "--power_cycle",
                    action="store_true", default=False, 
                    help="From remote host, power cycle the USB ports.")

argVars = parser.parse_args()

def killFW():
    if commonLib.isStUtilRunning():
        out = commonLib.exec_command("sudo pkill -f -9 st-util").split('\n')        

def killFW_remote(ssh, logger):
    logger.critical("Sending kill signal to remote host.")
    commonLib.exec_command_remote("sudo bash killFW.sh", ssh)
    logger.critical("Done.\n")

def executeBinary():
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)
    killFW()
    out = commonLib.exec_command("(st-util > stLog 2>&1)&")
    time.sleep(4)

    with open("stLog") as output:
        log = output.read()

    status = True
    if 'error' in log.lower():
        status = False
    assert status, ('ERROR - \n{}'.format(log))
    
    dirContent = os.listdir(".")
    currDir = os.getcwd()
    if "gdbCommands" not in dirContent:
        assert "gdbCommands" in dirContent,\
                "Error - missing gdbCommands file in dir\n{}".format(currDir)

    out = commonLib.exec_command("(arm-none-eabi-gdb -x gdbCommands)&")
    if "error" in out.lower():
        raise AssertionError("ERROR: executeBinary() - failed"
                             "\n\n{}".format(out))
    return out

def fwupload(logger):
    logger.info("Uploading image into remote device...")
    currentDir = os.getcwd()
    sftp = commonLib.sftpConnect()
    src = currentDir + "/../../build/kernel.elf"
    dest = "/home/pi/firmware/bin/kernel.elf"
    logger.info("Uploading image from \n\nsrc={} to \ndest={}...\n".format(src, dest))
    commonLib.sftpPut(sftp, src, dest)
    logger.info("Done.")
    commonLib.sftpClose(sftp)

def executeBinaryRemote(ssh, logger):
    logger.info("Executing bin from remote host...")
    out = commonLib.exec_command_remote("(sudo st-util > stLog 2>&1)&", ssh)
    time.sleep(4)
    logger.info("Started gdb on remote host")
    logger.info("If program not respond to ctr+c, try ctr+z or ctr+\\,"
                " then manually kill the FW, remotely")
    out = commonLib.exec_command_remote_background("(cd /home/pi/firmware/bin "
                                                   "&& arm-none-eabi-gdb -x gdbCommands)&", ssh)
    assert out, "Error: executeBinaryRemote(....): {}".format(out)    

def rebootRemoteUSBDevice(ssh, logger):
    logger.critical("Power cycling device connected to host usb...\n")
    logger.critical("Power OFF")
    out = commonLib.exec_command_remote("sudo /home/pi/hub-ctrl.c/./hub-ctrl -h 0 -P 2 -p 0", ssh)
    time.sleep(1)
    out = commonLib.exec_command_remote("sudo /home/pi/hub-ctrl.c/./hub-ctrl -h 0 -P 2 -p 1", ssh)
    assert (out != "" or out != " "), "ERROR: Power Cycle failed"
    logger.critical("Power ON\n")
    logger.info("Power cycle success\n")

def readUART_remote(logger, ssh):
    currentDir = os.getcwd()

    logger.info("current dir = {}".format(currentDir))
    
    serialDevice = commonLib.findDeviceBySerial()
    logger.info("Found device: {}\n".format(serialDevice))
    
    jdata = {"device":serialDevice}
    logger.info("jdata: {}".format(jdata))
    with open("config.json", "w") as jfile:
      json.dump(jdata, jfile)

    commonLib.uploadFileToRemoteHostMainDir(logger, "config.json", currentDir)
    commonLib.uploadFileToRemoteHostMainDir(logger, "remoteUART.py", currentDir)

    logger.info("Now listienning to USB port...")
    out = commonLib.exec_command_remote_background("(sudo python remoteUART.py)&", ssh)
    logger.info("exec_command_remote_background(.....): ".format(out))

def main():
    try:
        ssh = None
        logger = commonLib.configLogger()
        fileName = "killFW.sh"
        currentDir = os.getcwd()

        sftp = commonLib.sftpConnect()
        logger.info("Uploading {} to remote host...".format(fileName))
        src = currentDir + "/{}".format(fileName)
        dest = "/home/pi/{}".format(fileName)
        commonLib.sftpPut(sftp, src, dest)
        logger.info("Done.")
        commonLib.sftpClose(sftp)

        if argVars.build:
            out = commonLib.exec_command("python ../../build/buildAll.py")
            assert not ("err" in out.lower()), "Error: command failed to execute.\n{}"\
                                      .format(out)
        if argVars.execute_local:
            out = executeBinary()
        
        if argVars.fwup:
            out = fwupload(logger)

        if argVars.execute_remote:
            ssh = commonLib.sshOpen()
            killFW_remote(ssh, logger)
            readUART_remote(logger, ssh)
            executeBinaryRemote(ssh, logger)

        if argVars.killrfw:
            ssh = commonLib.sshOpen()
            killFW_remote(ssh, logger)

        if argVars.power_cycle:
            ssh = commonLib.sshOpen()
            rebootRemoteUSBDevice(ssh, logger)
    except:
        traceback.print_exc()
        killFW_remote(ssh, logger)
    finally:
        logger.critical("Cleanning up open ssh session...\n")
        commonLib.sshClose(ssh)

if __name__ == "__main__":
    main()