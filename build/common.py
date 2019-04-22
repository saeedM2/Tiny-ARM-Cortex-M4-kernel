#!/usr/bin/env python

import os
import sys
import subprocess
import logging
import traceback
import shutil

from undecorated import undecorated
from functools import wraps

def configLogger(name="test"):
    # TODO: clear logs before starting 

    if os.path.exists("logs/"):
        shutil.rmtree("logs/")
    if not os.path.exists("logs/"):
        os.makedirs("logs/")

    format = logging.Formatter(fmt="[%(asctime)s] "
                                   "%(levelname)-s - "
                                   "%(message)s",
                                  datefmt="%Y-%m-%d %H:%M:%S")

    handler = logging.FileHandler("logs/scriptLog", mode="a")
    handler.setFormatter(format)
    screen_handler = logging.StreamHandler(stream=sys.stdout)
    screen_handler.setFormatter(format)
    logger = logging.getLogger(name)
    logger.setLevel(logging.DEBUG)
    logger.addHandler(handler)
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
