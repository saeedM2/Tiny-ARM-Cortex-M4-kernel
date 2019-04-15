import fwManager
import traceback
import logging
import sys
import os

os.chdir("../")
sys.path.append(os.getcwd())
sys.path.append(os.path.abspath("common"))

import commonLib

def main():
    logger = commonLib.configLogger()
    try:
        logger.debug('Debugging here')
        out = fwManager.gbdUploadBinary()
    except:
        traceback.print_exc()
    finally:
        fwManager.killFW()

if __name__ == "__main__":
    main()
