import sys
import os
import time

os.chdir("../")
sys.path.append(os.getcwd())
sys.path.append(os.path.abspath("common"))

import commonLib

def gbdUploadBinary():
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

    out = commonLib.exec_command("(arm-none-eabi-gdb -x gdbCommands)&")
    if "error" in out.lower():
        raise AssertionError("ERROR: gbdUploadBinary() - failed"
                             "\n\n{}".format(out))
    return out

def killFW():
    out = commonLib.exec_command("ps -aux | grep st-util").split('\n')
    proc = out[0]
    if 'grep' not in proc:
        out = commonLib.exec_command("sudo pkill -f -9 st-util").split('\n')
