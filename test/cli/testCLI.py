#!/usr/bin/env python

import sys, os
import subprocess
import traceback

os.chdir("../")
sys.path.append(os.getcwd())
sys.path.append(os.path.abspath("../common"))

import commonLib

path = commonLib.exec_command("pwd")
path = path.strip('\n')

def gdbInstanciate():
    global path
    armGdb = "cd ../../build/ && arm-none-eabi-gdb kernel.elf"
    gbdInstance = subprocess.Popen(armGdb, stdin=subprocess.PIPE, shell=True)
    return gbdInstance

# Global instance
gdb = gdbInstanciate()

def gbdWrite(command):
    global gdb
    command += '\n'
    gdb.stdin.write(command)

def readUART():
    gbd.stdin.write('')
    commonLib.exec_command("(nohup st-util >)&")
def main():

    try:
        if "/cli" not in path:
            raise AssertionError("ERROR: exec_command() - build all faild.\n\nOutput msg:\n{}".format(path))

    except:
        commonLib.print_now(traceback.print_exc())

if __name__ == "__main__":
   main()
