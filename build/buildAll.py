#! /usr/bin/env python

import os
import subprocess
import common
import traceback

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

path = common.exec_command("pwd")
path = path.strip("\n")

def getBaseDir():
    out = os.listdir(".")
    while "Makefile" not in out:
        os.chdir("..")
        out = os.listdir(".")
    else:
        basePath = os.getcwd()
        return basePath

def bakeSrcCode(logger):
    path = getBaseDir()
    path = path + "/build"
    cmd = "make all 2>&1 | tee -a {}/logs/buildLog"
    output = common.exec_command(cmd.format(path))
    if "text	   data	    bss	    dec	    hex	filename" not in output:
        msg = "ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}"
        msgf = msg.format(output)
        logger.info(msgf)
        raise AssertionError(msgf)
    else:
        msg = "make all successful.\n"
        logger.info(msg)
        
def uploadBinary(logger):
    cmd = "st-flash write {}/kernel.bin 0x8000000 2>&1 | tee -a {}/logs/buildLog"
    output = common.exec_command(cmd.format(path, dname))
    if "Flash written and verified! jolly good!" not in output:
        msg = "ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}"
        msgf = msg.format(output)
        logger.info(msgf)
        raise AssertionError(msgf)
    else:
        logger.info("Binary successfully written to flash.\n")

def main():
    global path

    logger = common.configLogger()
    common.init(logger)

    try:
        if "/build" not in path:
            msg = ("ERROR: exec_command() -"
                   " build all faild.\n\nOutput msg:\n{}")
            msgf = msg.format(path)
            logger.info(msgf)
            raise AssertionError(msgf)
        bakeSrcCode(logger)
    except:
        logger.info(traceback.print_exc())

if __name__ == "__main__":
   main()
