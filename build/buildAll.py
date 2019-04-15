#! /usr/bin/env python

import os
import subprocess
import common
import traceback

path = common.exec_command("pwd")
path = path.strip('\n')

def bakeSrcCode():
    global path
    cmd = "cd .. && make all 2>&1 | tee -a {}/logs/buildLog"
    output = common.exec_command(cmd.format(path))
    if "text	   data	    bss	    dec	    hex	filename" not in output:
        msg = "ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}"
        raise AssertionError(msg.format(output))

def uploadBinary():
    cmd = "st-flash write kernel.bin 0x8000000 2>&1 | tee -a {}/logs/buildLog"
    output = common.exec_command(cmd.format(path))
    if "Flash written and verified! jolly good!" not in output:
        msg = "ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}"
        raise AssertionError(msg.format(output))

def main():
    global path

    common.init()

    try:

        if "/build" not in path:
            msg = ("ERROR: exec_command() -"
                   " build all faild.\n\nOutput msg:\n{}")
            raise AssertionError(.format(path))

        bakeSrcCode()
        uploadBinary()
    except:
        common.print_now(traceback.print_exc())

if __name__ == "__main__":
   main()
