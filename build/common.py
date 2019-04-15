#!/usr/bin/env python

import os
import subprocess
import logging
import traceback

from undecorated import undecorated
from functools import wraps

if not os.path.exists("logs/"):
    os.makedirs("logs/")
logging.basicConfig(filename="logs/debugMsg.log",
                    format="%(asctime)s %(levelname)-8s %(message)s",
                    level=logging.INFO,
                    datefmt="%Y-%m-%d %H:%M:%S")

def init():
    try:
        output = exec_command("rm logs/buildLog")
        if "" not in output:
            msg ("ERROR: exec_command() -"
                 " build all faild.\n\nOutput msg:\n{}")
            raise AssertionError(.format(output))
    except:
        common.print_now(traceback.print_exc())

# Unbuffered print
def print_now(text):
    try:
        sys.stdout.write(str(text) + '\r\n')
        sys.stdout.flush()
    except:
        pass

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

        fh.write("Time it takes to complete {}: {}\n".format(fname, delta))

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
                 logCriticalEnable=True,
                 logAllEnable=False,
                 logTimeEnable=False):
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
