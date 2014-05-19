EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:freewatch
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "Freewatch Peripherals & Sensors"
Date ""
Rev "v1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L L70 U?
U 1 1 537A7ED7
P 2300 1600
F 0 "U?" H 3000 1750 60  0000 C CNN
F 1 "L70" H 3000 450 60  0000 C CNN
F 2 "" H 2300 1600 60  0000 C CNN
F 3 "" H 2300 1600 60  0000 C CNN
	1    2300 1600
	1    0    0    -1  
$EndComp
$Comp
L LSM303C U?
U 1 1 537A7F34
P 5750 1650
F 0 "U?" H 6550 1800 60  0000 C CNN
F 1 "LSM303C" H 6550 900 60  0000 C CNN
F 2 "" H 7350 1650 60  0000 C CNN
F 3 "" H 7350 1650 60  0000 C CNN
	1    5750 1650
	1    0    0    -1  
$EndComp
$Comp
L MAX44009 U?
U 1 1 537A7F7B
P 2300 4250
F 0 "U?" H 2850 4400 60  0000 C CNN
F 1 "MAX44009" H 2850 3800 60  0000 C CNN
F 2 "" H 2300 4250 60  0000 C CNN
F 3 "" H 2300 4250 60  0000 C CNN
	1    2300 4250
	1    0    0    -1  
$EndComp
$Comp
L MICRO_SDCARD_SPI U?
U 1 1 537A7FBA
P 2650 5650
F 0 "U?" H 3200 5800 60  0000 C CNN
F 1 "MICRO_SDCARD_SPI" H 3100 4800 60  0000 C CNN
F 2 "" H 2650 5650 60  0000 C CNN
F 3 "" H 2650 5650 60  0000 C CNN
	1    2650 5650
	1    0    0    -1  
$EndComp
$Comp
L MPL3115A2 U?
U 1 1 537A8023
P 5300 3750
F 0 "U?" H 5900 3900 60  0000 C CNN
F 1 "MPL3115A2" H 5850 3300 60  0000 C CNN
F 2 "" H 5300 3750 60  0000 C CNN
F 3 "" H 5300 3750 60  0000 C CNN
	1    5300 3750
	1    0    0    -1  
$EndComp
$Comp
L W3011A ANT?
U 1 1 537A8050
P 1950 1600
F 0 "ANT?" H 2500 1750 60  0000 C CNN
F 1 "W3011A" H 2500 1350 60  0000 C CNN
F 2 "" H 1950 1600 60  0000 C CNN
F 3 "" H 1950 1600 60  0000 C CNN
	1    1950 1600
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
