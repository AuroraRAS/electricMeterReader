#/bin/sh

sdcc-sdcc -lstm8 -mstm8 -I/usr/share/sdcc/include --out-fmt-ihx main.c &&
../stm8flash/stm8flash -c stlinkv2 -p stm8s105k4 -w main.ihx
