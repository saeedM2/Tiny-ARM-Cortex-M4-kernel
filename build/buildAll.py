#! /usr/bin/env python

import os
import subprocess
import common
import traceback

path = common.exec_command("pwd")
path = path.strip('\n')

def bakeSrcCode():
    global path
    output = common.exec_command("cd .. && make all 2>&1 | tee -a {}/logs/buildLog".format(path))
    if "text	   data	    bss	    dec	    hex	filename" not in output:
        raise AssertionError("ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}".format(output))

def uploadBinary():
    output = common.exec_command("st-flash write kernel.bin 0x8000000 2>&1 | tee -a {}/logs/buildLog".format(path))
    if "Flash written and verified! jolly good!" not in output:
        raise AssertionError("ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}".format(output))

def main():

    global path

    common.init()

    try:

        if "/build" not in path:
            raise AssertionError("ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}".format(path))

        bakeSrcCode()
        uploadBinary()
    except:
        common.print_now(traceback.print_exc())

if __name__ == "__main__":
   main()
