import sys
import os
import time
import traceback

from argparse import ArgumentParser

sys.path.append(os.getcwd())
sys.path.append(os.path.abspath("../common"))

import commonLib

parser = ArgumentParser()

parser.add_argument("-b", "--build", 
                    action="store_true", default=False,
                    help="Build elf firmare binary.")
parser.add_argument("-u", "--execute",
                    action="store_true", default=False, 
                    help="Execute binary from gdb.")

argVars = parser.parse_args()

def killFW():
    if commonLib.isStUtilRunning():
        out = commonLib.exec_command("sudo pkill -f -9 st-util").split('\n')        

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

def main():
    try:
        if argVars.build:
            out = commonLib.exec_command("python ../../build/buildAll.py")
            assert not ("err" in out.lower()), "Error: command failed to execute.\n{}"\
                                      .format(out)
        if argVars.execute:
            out = executeBinary()
    except:
        traceback.print_exc()

if __name__ == "__main__":
    main()