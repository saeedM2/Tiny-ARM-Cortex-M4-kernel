
# Kill st-util tool if running
if pgrep "st-util"
then
        sudo killall st-util
fi

# Kill arm-none-eabi-gbd if running 
if pgrep "arm-none"
then
        sudo killall arm-none-eabi-gdb
fi
