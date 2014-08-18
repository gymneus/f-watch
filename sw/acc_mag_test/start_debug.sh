sudo openocd -s ../common/openocd -f interface/stlink-v2-1.cfg -f init.cfg &
/opt/gcc-arm-none-eabi-4_8-2014q1/bin/arm-none-eabi-gdb -ex "target remote localhost:3333" exe/freewatch.out
