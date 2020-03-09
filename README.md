# electricMeterReader
a DL/T645-2007 or DL/T645-1997 meter reader base on STM8S.

# try
To try the demo, you need `sdcc` and `stm8flash`. You can find `sdcc` in your Linux distribution's software repository, and find `stm8flash` [here](https://github.com/vdudouyt/stm8flash.git).
## build
`sdcc-sdcc -lstm8 -mstm8 -I/usr/share/sdcc/include --out-fmt-ihx main.c`

## flash
`stm8flash -c stlinkv2 -p stm8s105k4 -w main.ihx`

## read meter data in flash
`stm8flash -c stlinkv2 -p stm8s105k4 -s eeprom -r eeprom.dat`

![front](https://raw.githubusercontent.com/MiniLight/electricMeterReader/master/IMG_20170516_162147.jpg)
![back](https://raw.githubusercontent.com/MiniLight/electricMeterReader/master/IMG_20170516_162202.jpg)
