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
LIBS:freewatch_v1-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title "Freewatch MCU & Display"
Date ""
Rev "v1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L EFM32GG330 U15
U 1 1 537A82FC
P 3250 2050
F 0 "U15" H 4050 2200 60  0000 C CNN
F 1 "EFM32GG330" H 4100 -1700 60  0000 C CNN
F 2 "" H 3250 2050 60  0000 C CNN
F 3 "" H 3250 2050 60  0000 C CNN
	1    3250 2050
	1    0    0    -1  
$EndComp
$Comp
L LS013B7DH03 U14
U 1 1 537A8339
P 9650 1350
F 0 "U14" H 10100 1500 60  0000 C CNN
F 1 "LS013B7DH03" H 10100 300 60  0000 C CNN
F 2 "" H 9650 1350 60  0000 C CNN
F 3 "" H 9650 1350 60  0000 C CNN
	1    9650 1350
	1    0    0    -1  
$EndComp
Text Notes 600  1200 0    50   ~ 0
Copyright Julian Lewis 2014.\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.2.\nYou may redistribute and modify this documentation under the terms of the\nCERN OHL v.1.2. (http://ohwr.org/cernohl). This documentation is distributed\nWITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF MERCHANTABILITY,\nSATISFACTORY QUALITY AND FITNESS FOR A PARTICULAR PURPOSE.\nPlease see the CERN OHL v.1.2 for applicable conditions
Text GLabel 2850 4850 0    40   Output ~ 0
AC_CLK
Text GLabel 2850 5050 0    40   BiDi ~ 0
AC_SDIO
Text GLabel 2850 4750 0    40   Output ~ 0
AC_CS_XL
Text GLabel 2850 4950 0    40   Output ~ 0
AC_CS_MAG
Text GLabel 2850 2950 0    40   Input ~ 0
AC_INT_MAG
Text GLabel 5750 2450 2    40   Input ~ 0
AC_DRDY_MAG
Text GLabel 1150 4400 0    40   Output ~ 0
SCL
Text GLabel 1150 4300 0    40   BiDi ~ 0
SDA
Text GLabel 2850 2650 0    40   Input ~ 0
LS_INT_N
Text GLabel 5750 3750 2    40   Input ~ 0
GPS_TXD
Text GLabel 5750 3650 2    40   Output ~ 0
GPS_RXD
Text GLabel 5750 3550 2    40   Output ~ 0
GPS_ON_OFF
Text GLabel 5750 4250 2    40   Output ~ 0
GPS_RST
Text GLabel 7700 5950 2    40   Input ~ 0
GPS_PPS
Text GLabel 5750 2150 2    40   Input ~ 0
SD_MISO
Text GLabel 5750 2050 2    40   Output ~ 0
SD_MOSI
Text GLabel 5750 2250 2    40   Output ~ 0
SD_CLK
Text GLabel 5750 2350 2    40   Output ~ 0
SD_CS_N
Text GLabel 2850 2150 0    40   Input ~ 0
GPS_WAKUP
Text GLabel 5750 4450 2    40   BiDi ~ 0
USB_DP
Text GLabel 5750 4350 2    40   BiDi ~ 0
USB_DM
Text GLabel 5750 4550 2    40   Input ~ 0
USB_ID
Text GLabel 5750 2750 2    40   Input ~ 0
BC_PGOOD_N
Text GLabel 5750 2650 2    40   Input ~ 0
BC_CHG_N
Text GLabel 5750 2850 2    40   Input ~ 0
FG_ALRT_N
Text Notes 9200 2700 0    40   ~ 0
VCOM inversion\nEXTMODE = VDD -> hardware (EXTCOMIN)\nEXTMODE = VSS -> software
$Comp
L CAP C29
U 1 1 537D98E4
P 9050 2000
F 0 "C29" V 9000 2100 39  0000 C CNN
F 1 "100nF" V 9100 2100 39  0000 C CNN
F 2 "" H 9050 2000 60  0000 C CNN
F 3 "" H 9050 2000 60  0000 C CNN
	1    9050 2000
	0    1    1    0   
$EndComp
$Comp
L CAP C28
U 1 1 537D9904
P 8800 2000
F 0 "C28" V 8750 2100 39  0000 C CNN
F 1 "1uF" V 8850 2100 39  0000 C CNN
F 2 "" H 8800 2000 60  0000 C CNN
F 3 "" H 8800 2000 60  0000 C CNN
	1    8800 2000
	0    1    1    0   
$EndComp
$Comp
L GND #PWR066
U 1 1 537D9A0B
P 9550 2300
F 0 "#PWR066" H 9550 2300 30  0001 C CNN
F 1 "GND" H 9550 2230 30  0001 C CNN
F 2 "" H 9550 2300 60  0000 C CNN
F 3 "" H 9550 2300 60  0000 C CNN
	1    9550 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR067
U 1 1 537DC267
P 8800 2150
F 0 "#PWR067" H 8800 2150 30  0001 C CNN
F 1 "GND" H 8800 2080 30  0001 C CNN
F 2 "" H 8800 2150 60  0000 C CNN
F 3 "" H 8800 2150 60  0000 C CNN
	1    8800 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR068
U 1 1 537DC279
P 9050 2150
F 0 "#PWR068" H 9050 2150 30  0001 C CNN
F 1 "GND" H 9050 2080 30  0001 C CNN
F 2 "" H 9050 2150 60  0000 C CNN
F 3 "" H 9050 2150 60  0000 C CNN
	1    9050 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR069
U 1 1 537DC28B
P 9300 2150
F 0 "#PWR069" H 9300 2150 30  0001 C CNN
F 1 "GND" H 9300 2080 30  0001 C CNN
F 2 "" H 9300 2150 60  0000 C CNN
F 3 "" H 9300 2150 60  0000 C CNN
	1    9300 2150
	1    0    0    -1  
$EndComp
Text Label 8500 1350 0    40   ~ 0
LCD_CLK
Text Label 8500 1450 0    40   ~ 0
LCD_DI
Text Label 8500 1550 0    40   ~ 0
LCD_CS
Text Label 8500 1650 0    40   ~ 0
LCD_INV
Text Label 8500 1750 0    40   ~ 0
LCD_EN
$Comp
L CAP C30
U 1 1 537D9885
P 9300 2000
F 0 "C30" V 9250 2100 39  0000 C CNN
F 1 "100nF" V 9350 2100 39  0000 C CNN
F 2 "" H 9300 2000 60  0000 C CNN
F 3 "" H 9300 2000 60  0000 C CNN
	1    9300 2000
	0    1    1    0   
$EndComp
$Comp
L CAP C42
U 1 1 537DCB28
P 3050 5800
F 0 "C42" V 3000 5900 39  0000 C CNN
F 1 "100nF" V 3100 5900 39  0000 C CNN
F 2 "" H 3050 5800 60  0000 C CNN
F 3 "" H 3050 5800 60  0000 C CNN
	1    3050 5800
	0    1    1    0   
$EndComp
$Comp
L CAP C41
U 1 1 537DCC14
P 2800 5800
F 0 "C41" V 2750 5900 39  0000 C CNN
F 1 "100nF" V 2850 5900 39  0000 C CNN
F 2 "" H 2800 5800 60  0000 C CNN
F 3 "" H 2800 5800 60  0000 C CNN
	1    2800 5800
	0    1    1    0   
$EndComp
$Comp
L CAP C40
U 1 1 537DCC3D
P 2550 5800
F 0 "C40" V 2500 5900 39  0000 C CNN
F 1 "100nF" V 2600 5900 39  0000 C CNN
F 2 "" H 2550 5800 60  0000 C CNN
F 3 "" H 2550 5800 60  0000 C CNN
	1    2550 5800
	0    1    1    0   
$EndComp
$Comp
L CAP C39
U 1 1 537DCC6D
P 2300 5800
F 0 "C39" V 2250 5900 39  0000 C CNN
F 1 "100nF" V 2350 5900 39  0000 C CNN
F 2 "" H 2300 5800 60  0000 C CNN
F 3 "" H 2300 5800 60  0000 C CNN
	1    2300 5800
	0    1    1    0   
$EndComp
$Comp
L CAP C38
U 1 1 537DCC98
P 2050 5800
F 0 "C38" V 2000 5900 39  0000 C CNN
F 1 "100nF" V 2100 5900 39  0000 C CNN
F 2 "" H 2050 5800 60  0000 C CNN
F 3 "" H 2050 5800 60  0000 C CNN
	1    2050 5800
	0    1    1    0   
$EndComp
$Comp
L CAP C33
U 1 1 537DCCC0
P 5150 5600
F 0 "C33" V 5100 5700 39  0000 C CNN
F 1 "1uF" V 5200 5700 39  0000 C CNN
F 2 "" H 5150 5600 60  0000 C CNN
F 3 "" H 5150 5600 60  0000 C CNN
	1    5150 5600
	0    1    1    0   
$EndComp
$Comp
L CAP C34
U 1 1 537DCD31
P 5400 5600
F 0 "C34" V 5350 5700 39  0000 C CNN
F 1 "100nF" V 5450 5700 39  0000 C CNN
F 2 "" H 5400 5600 60  0000 C CNN
F 3 "" H 5400 5600 60  0000 C CNN
	1    5400 5600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR070
U 1 1 537DD069
P 5150 5750
F 0 "#PWR070" H 5150 5750 30  0001 C CNN
F 1 "GND" H 5150 5680 30  0001 C CNN
F 2 "" H 5150 5750 60  0000 C CNN
F 3 "" H 5150 5750 60  0000 C CNN
	1    5150 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR071
U 1 1 537DD0BA
P 5000 5750
F 0 "#PWR071" H 5000 5750 30  0001 C CNN
F 1 "GND" H 5000 5680 30  0001 C CNN
F 2 "" H 5000 5750 60  0000 C CNN
F 3 "" H 5000 5750 60  0000 C CNN
	1    5000 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR072
U 1 1 537DD0D3
P 5400 5750
F 0 "#PWR072" H 5400 5750 30  0001 C CNN
F 1 "GND" H 5400 5680 30  0001 C CNN
F 2 "" H 5400 5750 60  0000 C CNN
F 3 "" H 5400 5750 60  0000 C CNN
	1    5400 5750
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR073
U 1 1 537DD288
P 5400 5300
F 0 "#PWR073" H 5400 5400 30  0001 C CNN
F 1 "VCC" H 5400 5400 30  0000 C CNN
F 2 "" H 5400 5300 60  0000 C CNN
F 3 "" H 5400 5300 60  0000 C CNN
	1    5400 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR074
U 1 1 537DD40A
P 3050 5950
F 0 "#PWR074" H 3050 5950 30  0001 C CNN
F 1 "GND" H 3050 5880 30  0001 C CNN
F 2 "" H 3050 5950 60  0000 C CNN
F 3 "" H 3050 5950 60  0000 C CNN
	1    3050 5950
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR075
U 1 1 537DD423
P 1800 5600
F 0 "#PWR075" H 1800 5700 30  0001 C CNN
F 1 "VCC" H 1800 5700 30  0000 C CNN
F 2 "" H 1800 5600 60  0000 C CNN
F 3 "" H 1800 5600 60  0000 C CNN
	1    1800 5600
	1    0    0    -1  
$EndComp
$Comp
L CAP C37
U 1 1 537DD458
P 1800 5800
F 0 "C37" V 1750 5900 39  0000 C CNN
F 1 "10uF" V 1850 5900 39  0000 C CNN
F 2 "" H 1800 5800 60  0000 C CNN
F 3 "" H 1800 5800 60  0000 C CNN
	1    1800 5800
	0    1    1    0   
$EndComp
$Comp
L GND #PWR076
U 1 1 537DD77E
P 2800 5950
F 0 "#PWR076" H 2800 5950 30  0001 C CNN
F 1 "GND" H 2800 5880 30  0001 C CNN
F 2 "" H 2800 5950 60  0000 C CNN
F 3 "" H 2800 5950 60  0000 C CNN
	1    2800 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR077
U 1 1 537DD798
P 2550 5950
F 0 "#PWR077" H 2550 5950 30  0001 C CNN
F 1 "GND" H 2550 5880 30  0001 C CNN
F 2 "" H 2550 5950 60  0000 C CNN
F 3 "" H 2550 5950 60  0000 C CNN
	1    2550 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR078
U 1 1 537DD7B2
P 2300 5950
F 0 "#PWR078" H 2300 5950 30  0001 C CNN
F 1 "GND" H 2300 5880 30  0001 C CNN
F 2 "" H 2300 5950 60  0000 C CNN
F 3 "" H 2300 5950 60  0000 C CNN
	1    2300 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR079
U 1 1 537DD7CC
P 2050 5950
F 0 "#PWR079" H 2050 5950 30  0001 C CNN
F 1 "GND" H 2050 5880 30  0001 C CNN
F 2 "" H 2050 5950 60  0000 C CNN
F 3 "" H 2050 5950 60  0000 C CNN
	1    2050 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR080
U 1 1 537DD7E6
P 1800 5950
F 0 "#PWR080" H 1800 5950 30  0001 C CNN
F 1 "GND" H 1800 5880 30  0001 C CNN
F 2 "" H 1800 5950 60  0000 C CNN
F 3 "" H 1800 5950 60  0000 C CNN
	1    1800 5950
	1    0    0    -1  
$EndComp
$Comp
L CAP C35
U 1 1 537D2590
P 5650 5600
F 0 "C35" V 5600 5700 39  0000 C CNN
F 1 "4.7uF" V 5700 5700 39  0000 C CNN
F 2 "" H 5650 5600 60  0000 C CNN
F 3 "" H 5650 5600 60  0000 C CNN
	1    5650 5600
	0    1    1    0   
$EndComp
$Comp
L CAP C36
U 1 1 537D25C8
P 5900 5600
F 0 "C36" V 5850 5700 39  0000 C CNN
F 1 "1uF" V 5950 5700 39  0000 C CNN
F 2 "" H 5900 5600 60  0000 C CNN
F 3 "" H 5900 5600 60  0000 C CNN
	1    5900 5600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR081
U 1 1 537D2613
P 5650 5750
F 0 "#PWR081" H 5650 5750 30  0001 C CNN
F 1 "GND" H 5650 5680 30  0001 C CNN
F 2 "" H 5650 5750 60  0000 C CNN
F 3 "" H 5650 5750 60  0000 C CNN
	1    5650 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR082
U 1 1 537D262F
P 5900 5750
F 0 "#PWR082" H 5900 5750 30  0001 C CNN
F 1 "GND" H 5900 5680 30  0001 C CNN
F 2 "" H 5900 5750 60  0000 C CNN
F 3 "" H 5900 5750 60  0000 C CNN
	1    5900 5750
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR083
U 1 1 537D291C
P 5150 4900
F 0 "#PWR083" H 5150 4990 20  0001 C CNN
F 1 "+5V" H 5150 4990 30  0000 C CNN
F 2 "" H 5150 4900 60  0000 C CNN
F 3 "" H 5150 4900 60  0000 C CNN
	1    5150 4900
	1    0    0    -1  
$EndComp
$Comp
L RES R21
U 1 1 537D2D0A
P 5150 4350
F 0 "R21" H 5000 4300 39  0000 C CNN
F 1 "15R" H 5150 4350 30  0000 C CNN
F 2 "" H 5150 4350 60  0000 C CNN
F 3 "" H 5150 4350 60  0000 C CNN
	1    5150 4350
	-1   0    0    1   
$EndComp
$Comp
L RES R22
U 1 1 537D2D6B
P 5150 4450
F 0 "R22" H 5000 4400 39  0000 C CNN
F 1 "15R" H 5150 4450 30  0000 C CNN
F 2 "" H 5150 4450 60  0000 C CNN
F 3 "" H 5150 4450 60  0000 C CNN
	1    5150 4450
	-1   0    0    1   
$EndComp
Text Label 5400 4350 0    40   ~ 0
USB_DM
Text Label 5400 4450 0    40   ~ 0
USB_DP
Text Label 5400 4550 0    40   ~ 0
USB_ID
Text Label 5750 4750 0    40   ~ 0
MCU_RST_N
Text GLabel 2850 4250 0    40   Output ~ 0
SD_EN
$Comp
L CRYSTAL_SMD X1
U 1 1 537FC26F
P 1250 3300
F 0 "X1" V 1200 3400 30  0000 C CNN
F 1 "32.768kHz" V 1300 3375 30  0000 L CNN
F 2 "" H 1250 3300 60  0000 C CNN
F 3 "" H 1250 3300 60  0000 C CNN
	1    1250 3300
	0    1    1    0   
$EndComp
Text Notes 1050 3650 0    40   ~ 0
Do not solder\nthe crystal body!
$Comp
L CAP C31
U 1 1 537FCACE
P 900 3100
F 0 "C31" V 850 3200 39  0000 C CNN
F 1 "18pF" V 950 3200 39  0000 C CNN
F 2 "" H 900 3100 60  0000 C CNN
F 3 "" H 900 3100 60  0000 C CNN
	1    900  3100
	1    0    0    -1  
$EndComp
$Comp
L CAP C32
U 1 1 537FCB4C
P 900 3500
F 0 "C32" V 850 3600 39  0000 C CNN
F 1 "18pF" V 950 3600 39  0000 C CNN
F 2 "" H 900 3500 60  0000 C CNN
F 3 "" H 900 3500 60  0000 C CNN
	1    900  3500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR084
U 1 1 537FCDAB
P 700 3550
F 0 "#PWR084" H 700 3550 30  0001 C CNN
F 1 "GND" H 700 3480 30  0001 C CNN
F 2 "" H 700 3550 60  0000 C CNN
F 3 "" H 700 3550 60  0000 C CNN
	1    700  3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR085
U 1 1 537FCDFB
P 700 3150
F 0 "#PWR085" H 700 3150 30  0001 C CNN
F 1 "GND" H 700 3080 30  0001 C CNN
F 2 "" H 700 3150 60  0000 C CNN
F 3 "" H 700 3150 60  0000 C CNN
	1    700  3150
	1    0    0    -1  
$EndComp
NoConn ~ 1100 3300
$Comp
L RES R15
U 1 1 53824411
P 1250 4150
F 0 "R15" H 1220 4210 39  0000 C CNN
F 1 "3k3" H 1250 4150 30  0000 C CNN
F 2 "" H 1250 4150 60  0000 C CNN
F 3 "" H 1250 4150 60  0000 C CNN
	1    1250 4150
	0    1    1    0   
$EndComp
$Comp
L RES R16
U 1 1 5382452D
P 1450 4150
F 0 "R16" H 1420 4210 39  0000 C CNN
F 1 "3k3" H 1450 4150 30  0000 C CNN
F 2 "" H 1450 4150 60  0000 C CNN
F 3 "" H 1450 4150 60  0000 C CNN
	1    1450 4150
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR086
U 1 1 538245F4
P 1250 4000
F 0 "#PWR086" H 1250 4100 30  0001 C CNN
F 1 "VCC" H 1250 4100 30  0000 C CNN
F 2 "" H 1250 4000 60  0000 C CNN
F 3 "" H 1250 4000 60  0000 C CNN
	1    1250 4000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR087
U 1 1 53824663
P 1450 4000
F 0 "#PWR087" H 1450 4100 30  0001 C CNN
F 1 "VCC" H 1450 4100 30  0000 C CNN
F 2 "" H 1450 4000 60  0000 C CNN
F 3 "" H 1450 4000 60  0000 C CNN
	1    1450 4000
	1    0    0    -1  
$EndComp
Text Notes 1000 3850 0    40   ~ 0
Check pull-ups value !!
Text Label 5750 3950 0    40   ~ 0
SWCLK
Text Label 5750 4050 0    40   ~ 0
SWDIO
Text Label 5750 4150 0    40   ~ 0
SWDO
Text Label 2850 4350 0    40   ~ 0
LCD_CLK
Text Label 2850 4150 0    40   ~ 0
LCD_DI
Text Label 2850 2250 0    40   ~ 0
LCD_INV
Text Label 2850 4450 0    40   ~ 0
LCD_CS
Text Label 2850 3950 0    40   ~ 0
LCD_EN
Text Label 8500 1850 0    40   ~ 0
LCD_VCC
Text Label 2850 4050 0    40   ~ 0
LCD_VCC
Text Notes 1350 5000 0    40   ~ 0
Check that US0_RX (PC10) can be\nused as GPIO when using USART0
$Comp
L CONN_6 P7
U 1 1 5382C001
P 9650 5800
F 0 "P7" V 9600 5800 60  0000 C CNN
F 1 "CONN_6" V 9700 5800 60  0000 C CNN
F 2 "" H 9650 5800 60  0000 C CNN
F 3 "" H 9650 5800 60  0000 C CNN
	1    9650 5800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR088
U 1 1 5382C124
P 9200 5500
F 0 "#PWR088" H 9200 5600 30  0001 C CNN
F 1 "VCC" H 9200 5600 30  0000 C CNN
F 2 "" H 9200 5500 60  0000 C CNN
F 3 "" H 9200 5500 60  0000 C CNN
	1    9200 5500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR089
U 1 1 5382C149
P 9200 6100
F 0 "#PWR089" H 9200 6100 30  0001 C CNN
F 1 "GND" H 9200 6030 30  0001 C CNN
F 2 "" H 9200 6100 60  0000 C CNN
F 3 "" H 9200 6100 60  0000 C CNN
	1    9200 6100
	1    0    0    -1  
$EndComp
Text Label 8900 5650 0    40   ~ 0
SWDIO
Text Label 8900 5750 0    40   ~ 0
SWCLK
Text Label 8900 5850 0    40   ~ 0
SWDO
Text Label 8900 5950 0    40   ~ 0
MCU_RST_N
Wire Notes Line
	550  550  550  1250
Wire Notes Line
	550  1250 3700 1250
Wire Notes Line
	3700 1250 3700 550 
Wire Notes Line
	3700 550  550  550 
Wire Wire Line
	8500 1750 9650 1750
Wire Wire Line
	8500 1850 9650 1850
Wire Wire Line
	9050 1850 9050 1900
Wire Wire Line
	8800 2100 8800 2150
Wire Wire Line
	9050 2100 9050 2150
Wire Wire Line
	9300 2100 9300 2150
Wire Wire Line
	9650 2150 9550 2150
Wire Wire Line
	9550 2150 9550 2300
Wire Wire Line
	9650 2250 9550 2250
Connection ~ 9550 2250
Wire Wire Line
	9550 1850 9550 2050
Connection ~ 9550 1850
Wire Wire Line
	9550 1950 9650 1950
Connection ~ 9050 1850
Wire Wire Line
	9650 1650 8500 1650
Wire Wire Line
	9650 1550 8500 1550
Wire Wire Line
	9650 1450 8500 1450
Wire Wire Line
	9650 1350 8500 1350
Wire Wire Line
	9550 2050 9650 2050
Connection ~ 9550 1950
Wire Wire Line
	9300 1900 9300 1750
Connection ~ 9300 1750
Wire Wire Line
	8800 1900 8800 1850
Connection ~ 8800 1850
Wire Wire Line
	4950 5650 5000 5650
Wire Wire Line
	5000 5650 5000 5750
Wire Wire Line
	5150 5700 5150 5750
Wire Wire Line
	5400 5700 5400 5750
Wire Wire Line
	4950 5450 5150 5450
Wire Wire Line
	5150 5450 5150 5500
Wire Wire Line
	4950 5350 5400 5350
Wire Wire Line
	5400 5300 5400 5500
Connection ~ 5400 5350
Wire Wire Line
	3050 5900 3050 5950
Wire Wire Line
	2800 5900 2800 5950
Wire Wire Line
	2550 5900 2550 5950
Wire Wire Line
	2300 5900 2300 5950
Wire Wire Line
	2050 5900 2050 5950
Wire Wire Line
	1800 5900 1800 5950
Wire Wire Line
	3250 5250 3050 5250
Wire Wire Line
	3050 5250 3050 5700
Wire Wire Line
	1800 5650 3250 5650
Connection ~ 3050 5650
Wire Wire Line
	3250 5550 3050 5550
Connection ~ 3050 5550
Wire Wire Line
	3250 5450 3050 5450
Connection ~ 3050 5450
Wire Wire Line
	3250 5350 3050 5350
Connection ~ 3050 5350
Wire Wire Line
	1800 5600 1800 5700
Wire Wire Line
	2050 5650 2050 5700
Connection ~ 2050 5650
Wire Wire Line
	2300 5650 2300 5700
Connection ~ 2300 5650
Wire Wire Line
	2550 5650 2550 5700
Connection ~ 2550 5650
Wire Wire Line
	2800 5650 2800 5700
Connection ~ 2800 5650
Connection ~ 1800 5650
Wire Wire Line
	5650 5700 5650 5750
Wire Wire Line
	5900 5700 5900 5750
Wire Wire Line
	4950 5150 5650 5150
Wire Wire Line
	5150 4900 5150 5150
Wire Wire Line
	5150 4950 4950 4950
Wire Wire Line
	4950 5050 5900 5050
Wire Wire Line
	5650 5150 5650 5500
Connection ~ 5150 5150
Wire Wire Line
	5900 5050 5900 5500
Connection ~ 5150 4950
Wire Wire Line
	4950 4350 5050 4350
Wire Wire Line
	4950 4450 5050 4450
Wire Wire Line
	4950 4550 5750 4550
Wire Wire Line
	5250 4350 5750 4350
Wire Wire Line
	5250 4450 5750 4450
Wire Wire Line
	4950 4750 5750 4750
Wire Wire Line
	3250 3250 1700 3250
Wire Wire Line
	1700 3250 1700 3100
Wire Wire Line
	1700 3100 1000 3100
Wire Wire Line
	1000 3500 1700 3500
Wire Wire Line
	1700 3500 1700 3350
Wire Wire Line
	1700 3350 3250 3350
Connection ~ 1250 3100
Connection ~ 1250 3500
Wire Wire Line
	800  3100 700  3100
Wire Wire Line
	700  3100 700  3150
Wire Wire Line
	800  3500 700  3500
Wire Wire Line
	700  3500 700  3550
Wire Wire Line
	1150 3300 1100 3300
Wire Wire Line
	4950 2450 5750 2450
Wire Wire Line
	4950 2550 5750 2550
Wire Wire Line
	2850 2050 3250 2050
Wire Wire Line
	2850 2150 3250 2150
Wire Wire Line
	1250 4000 1250 4050
Wire Wire Line
	1450 4000 1450 4050
Wire Wire Line
	4950 3950 5750 3950
Wire Wire Line
	4950 4050 5750 4050
Wire Wire Line
	4950 4150 5750 4150
Wire Wire Line
	4950 3450 5750 3450
Wire Wire Line
	2850 4150 3250 4150
Wire Wire Line
	2850 4350 3250 4350
Wire Wire Line
	3250 4450 2850 4450
Wire Wire Line
	4950 2050 5750 2050
Wire Wire Line
	4950 2250 5750 2250
Wire Wire Line
	3250 2250 2850 2250
Wire Wire Line
	2850 2350 3250 2350
Wire Wire Line
	4950 2150 5750 2150
Wire Wire Line
	5750 2350 4950 2350
Wire Wire Line
	4950 3650 5750 3650
Wire Wire Line
	4950 3750 5750 3750
Wire Wire Line
	1800 3450 3250 3450
Wire Wire Line
	1900 3550 3250 3550
Wire Wire Line
	2850 4050 3250 4050
Wire Wire Line
	2850 5050 3250 5050
Wire Wire Line
	3250 4950 2850 4950
Wire Wire Line
	2850 4850 3250 4850
Wire Wire Line
	3250 4750 2850 4750
Wire Wire Line
	9200 5500 9200 5550
Wire Wire Line
	9200 5550 9300 5550
Wire Wire Line
	8900 5650 9300 5650
Wire Wire Line
	8900 5750 9300 5750
Wire Wire Line
	8900 5850 9300 5850
Wire Wire Line
	8900 5950 9300 5950
Wire Wire Line
	9300 6050 9200 6050
Wire Wire Line
	9200 6050 9200 6100
$Comp
L SW_PUSH SW1
U 1 1 5383007F
P 1500 7100
F 0 "SW1" H 1650 7210 50  0000 C CNN
F 1 "EVQ-P4MB3K" H 1500 7020 50  0000 C CNN
F 2 "" H 1500 7100 60  0000 C CNN
F 3 "" H 1500 7100 60  0000 C CNN
	1    1500 7100
	1    0    0    -1  
$EndComp
$Comp
L CAP C43
U 1 1 53830086
P 1900 7250
F 0 "C43" V 1850 7350 39  0000 C CNN
F 1 "100nF" V 1950 7350 39  0000 C CNN
F 2 "" H 1900 7250 60  0000 C CNN
F 3 "" H 1900 7250 60  0000 C CNN
	1    1900 7250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR090
U 1 1 5383008D
P 1100 7150
F 0 "#PWR090" H 1100 7150 30  0001 C CNN
F 1 "GND" H 1100 7080 30  0001 C CNN
F 2 "" H 1100 7150 60  0000 C CNN
F 3 "" H 1100 7150 60  0000 C CNN
	1    1100 7150
	1    0    0    -1  
$EndComp
$Comp
L RES R23
U 1 1 53830093
P 1900 6950
F 0 "R23" H 1870 7010 39  0000 C CNN
F 1 "100k" H 1900 6950 30  0000 C CNN
F 2 "" H 1900 6950 60  0000 C CNN
F 3 "" H 1900 6950 60  0000 C CNN
	1    1900 6950
	0    1    1    0   
$EndComp
$Comp
L GND #PWR091
U 1 1 5383009A
P 1900 7400
F 0 "#PWR091" H 1900 7400 30  0001 C CNN
F 1 "GND" H 1900 7330 30  0001 C CNN
F 2 "" H 1900 7400 60  0000 C CNN
F 3 "" H 1900 7400 60  0000 C CNN
	1    1900 7400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR092
U 1 1 538300A0
P 1900 6800
F 0 "#PWR092" H 1900 6900 30  0001 C CNN
F 1 "VCC" H 1900 6900 30  0000 C CNN
F 2 "" H 1900 6800 60  0000 C CNN
F 3 "" H 1900 6800 60  0000 C CNN
	1    1900 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 7150 1100 7100
Wire Wire Line
	1100 7100 1200 7100
Wire Wire Line
	1800 7100 2100 7100
Wire Wire Line
	1900 7050 1900 7150
Connection ~ 1900 7100
Wire Wire Line
	1900 6800 1900 6850
Wire Wire Line
	1900 7350 1900 7400
$Comp
L SW_PUSH SW2
U 1 1 538300B0
P 2900 7100
F 0 "SW2" H 3050 7210 50  0000 C CNN
F 1 "EVQ-P4MB3K" H 2900 7020 50  0000 C CNN
F 2 "" H 2900 7100 60  0000 C CNN
F 3 "" H 2900 7100 60  0000 C CNN
	1    2900 7100
	1    0    0    -1  
$EndComp
$Comp
L CAP C44
U 1 1 538300B7
P 3300 7250
F 0 "C44" V 3250 7350 39  0000 C CNN
F 1 "100nF" V 3350 7350 39  0000 C CNN
F 2 "" H 3300 7250 60  0000 C CNN
F 3 "" H 3300 7250 60  0000 C CNN
	1    3300 7250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR093
U 1 1 538300BE
P 2500 7150
F 0 "#PWR093" H 2500 7150 30  0001 C CNN
F 1 "GND" H 2500 7080 30  0001 C CNN
F 2 "" H 2500 7150 60  0000 C CNN
F 3 "" H 2500 7150 60  0000 C CNN
	1    2500 7150
	1    0    0    -1  
$EndComp
$Comp
L RES R24
U 1 1 538300C4
P 3300 6950
F 0 "R24" H 3270 7010 39  0000 C CNN
F 1 "100k" H 3300 6950 30  0000 C CNN
F 2 "" H 3300 6950 60  0000 C CNN
F 3 "" H 3300 6950 60  0000 C CNN
	1    3300 6950
	0    1    1    0   
$EndComp
$Comp
L GND #PWR094
U 1 1 538300CB
P 3300 7400
F 0 "#PWR094" H 3300 7400 30  0001 C CNN
F 1 "GND" H 3300 7330 30  0001 C CNN
F 2 "" H 3300 7400 60  0000 C CNN
F 3 "" H 3300 7400 60  0000 C CNN
	1    3300 7400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR095
U 1 1 538300D1
P 3300 6800
F 0 "#PWR095" H 3300 6900 30  0001 C CNN
F 1 "VCC" H 3300 6900 30  0000 C CNN
F 2 "" H 3300 6800 60  0000 C CNN
F 3 "" H 3300 6800 60  0000 C CNN
	1    3300 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 7150 2500 7100
Wire Wire Line
	2500 7100 2600 7100
Wire Wire Line
	3200 7100 3500 7100
Wire Wire Line
	3300 7050 3300 7150
Connection ~ 3300 7100
Wire Wire Line
	3300 6800 3300 6850
Wire Wire Line
	3300 7350 3300 7400
$Comp
L SW_PUSH SW3
U 1 1 538300E1
P 4300 7100
F 0 "SW3" H 4450 7210 50  0000 C CNN
F 1 "EVQ-P4MB3K" H 4300 7020 50  0000 C CNN
F 2 "" H 4300 7100 60  0000 C CNN
F 3 "" H 4300 7100 60  0000 C CNN
	1    4300 7100
	1    0    0    -1  
$EndComp
$Comp
L CAP C45
U 1 1 538300E8
P 4700 7250
F 0 "C45" V 4650 7350 39  0000 C CNN
F 1 "100nF" V 4750 7350 39  0000 C CNN
F 2 "" H 4700 7250 60  0000 C CNN
F 3 "" H 4700 7250 60  0000 C CNN
	1    4700 7250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR096
U 1 1 538300EF
P 3900 7150
F 0 "#PWR096" H 3900 7150 30  0001 C CNN
F 1 "GND" H 3900 7080 30  0001 C CNN
F 2 "" H 3900 7150 60  0000 C CNN
F 3 "" H 3900 7150 60  0000 C CNN
	1    3900 7150
	1    0    0    -1  
$EndComp
$Comp
L RES R25
U 1 1 538300F5
P 4700 6950
F 0 "R25" H 4670 7010 39  0000 C CNN
F 1 "100k" H 4700 6950 30  0000 C CNN
F 2 "" H 4700 6950 60  0000 C CNN
F 3 "" H 4700 6950 60  0000 C CNN
	1    4700 6950
	0    1    1    0   
$EndComp
$Comp
L GND #PWR097
U 1 1 538300FC
P 4700 7400
F 0 "#PWR097" H 4700 7400 30  0001 C CNN
F 1 "GND" H 4700 7330 30  0001 C CNN
F 2 "" H 4700 7400 60  0000 C CNN
F 3 "" H 4700 7400 60  0000 C CNN
	1    4700 7400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR098
U 1 1 53830102
P 4700 6800
F 0 "#PWR098" H 4700 6900 30  0001 C CNN
F 1 "VCC" H 4700 6900 30  0000 C CNN
F 2 "" H 4700 6800 60  0000 C CNN
F 3 "" H 4700 6800 60  0000 C CNN
	1    4700 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 7150 3900 7100
Wire Wire Line
	3900 7100 4000 7100
Wire Wire Line
	4600 7100 4900 7100
Wire Wire Line
	4700 7050 4700 7150
Connection ~ 4700 7100
Wire Wire Line
	4700 6800 4700 6850
Wire Wire Line
	4700 7350 4700 7400
$Comp
L SW_PUSH SW4
U 1 1 53830112
P 5700 7100
F 0 "SW4" H 5850 7210 50  0000 C CNN
F 1 "EVQ-P4MB3K" H 5700 7020 50  0000 C CNN
F 2 "" H 5700 7100 60  0000 C CNN
F 3 "" H 5700 7100 60  0000 C CNN
	1    5700 7100
	1    0    0    -1  
$EndComp
$Comp
L CAP C46
U 1 1 53830119
P 6100 7250
F 0 "C46" V 6050 7350 39  0000 C CNN
F 1 "100nF" V 6150 7350 39  0000 C CNN
F 2 "" H 6100 7250 60  0000 C CNN
F 3 "" H 6100 7250 60  0000 C CNN
	1    6100 7250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR099
U 1 1 53830120
P 5300 7150
F 0 "#PWR099" H 5300 7150 30  0001 C CNN
F 1 "GND" H 5300 7080 30  0001 C CNN
F 2 "" H 5300 7150 60  0000 C CNN
F 3 "" H 5300 7150 60  0000 C CNN
	1    5300 7150
	1    0    0    -1  
$EndComp
$Comp
L RES R26
U 1 1 53830126
P 6100 6950
F 0 "R26" H 6070 7010 39  0000 C CNN
F 1 "100k" H 6100 6950 30  0000 C CNN
F 2 "" H 6100 6950 60  0000 C CNN
F 3 "" H 6100 6950 60  0000 C CNN
	1    6100 6950
	0    1    1    0   
$EndComp
$Comp
L GND #PWR0100
U 1 1 5383012D
P 6100 7400
F 0 "#PWR0100" H 6100 7400 30  0001 C CNN
F 1 "GND" H 6100 7330 30  0001 C CNN
F 2 "" H 6100 7400 60  0000 C CNN
F 3 "" H 6100 7400 60  0000 C CNN
	1    6100 7400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR0101
U 1 1 53830133
P 6100 6800
F 0 "#PWR0101" H 6100 6900 30  0001 C CNN
F 1 "VCC" H 6100 6900 30  0000 C CNN
F 2 "" H 6100 6800 60  0000 C CNN
F 3 "" H 6100 6800 60  0000 C CNN
	1    6100 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 7150 5300 7100
Wire Wire Line
	5300 7100 5400 7100
Wire Wire Line
	6000 7100 6300 7100
Wire Wire Line
	6100 7050 6100 7150
Connection ~ 6100 7100
Wire Wire Line
	6100 6800 6100 6850
Wire Wire Line
	6100 7350 6100 7400
Text Label 2850 4650 0    40   ~ 0
SW_TL
Text Label 2850 2050 0    40   ~ 0
SW_TR
Text Label 2850 4550 0    40   ~ 0
SW_BL
Text Label 2850 2750 0    40   ~ 0
SW_BR
Text Label 2100 7100 0    40   ~ 0
SW_TL
Text Label 3500 7100 0    40   ~ 0
SW_TR
Text Label 4900 7100 0    40   ~ 0
SW_BL
Text Label 6300 7100 0    40   ~ 0
SW_BR
Text GLabel 2850 2350 0    40   Output ~ 0
VM_EN
Text Notes 9250 950  0    60   ~ 12
Memory LCD display
Text Notes 9050 5300 0    60   ~ 12
Debug interface
Text Notes 3500 6600 0    60   ~ 12
Buttons
Text Notes 3850 1700 0    60   ~ 12
The brain
Text Notes 9100 3050 0    60   ~ 12
LCD backlight
Text Notes 9150 4350 0    60   ~ 12
Buzzer
Text Notes 2100 2250 0    40   ~ 0
Timer 0 output 2 (PWM)
Text Label 5750 3450 0    40   ~ 0
LCD_BKL1
Text Label 5750 3350 0    40   ~ 0
LCD_BKL2
Text Label 8350 3200 0    40   ~ 0
LCD_BKL1
Text Label 8350 3350 0    40   ~ 0
LCD_BKL2
$Comp
L RES R17
U 1 1 5383BD39
P 9000 3500
F 0 "R17" H 8970 3560 39  0000 C CNN
F 1 "91R" H 9000 3500 30  0000 C CNN
F 2 "" H 9000 3500 60  0000 C CNN
F 3 "" H 9000 3500 60  0000 C CNN
	1    9000 3500
	0    1    1    0   
$EndComp
$Comp
L RES R18
U 1 1 5383BE91
P 9250 3500
F 0 "R18" H 9220 3560 39  0000 C CNN
F 1 "91R" H 9250 3500 30  0000 C CNN
F 2 "" H 9250 3500 60  0000 C CNN
F 3 "" H 9250 3500 60  0000 C CNN
	1    9250 3500
	0    1    1    0   
$EndComp
$Comp
L RES R19
U 1 1 5383BECD
P 9500 3500
F 0 "R19" H 9470 3560 39  0000 C CNN
F 1 "91R" H 9500 3500 30  0000 C CNN
F 2 "" H 9500 3500 60  0000 C CNN
F 3 "" H 9500 3500 60  0000 C CNN
	1    9500 3500
	0    1    1    0   
$EndComp
$Comp
L RES R20
U 1 1 5383BF0A
P 9750 3500
F 0 "R20" H 9720 3560 39  0000 C CNN
F 1 "91R" H 9750 3500 30  0000 C CNN
F 2 "" H 9750 3500 60  0000 C CNN
F 3 "" H 9750 3500 60  0000 C CNN
	1    9750 3500
	0    1    1    0   
$EndComp
$Comp
L LED_STD D4
U 1 1 5383CA02
P 9000 3800
F 0 "D4" H 9000 3875 39  0000 C CNN
F 1 "LED_STD" H 9000 3700 30  0000 C CNN
F 2 "" H 9000 3800 60  0000 C CNN
F 3 "" H 9000 3800 60  0000 C CNN
	1    9000 3800
	0    1    1    0   
$EndComp
$Comp
L LED_STD D5
U 1 1 5383CB1F
P 9250 3800
F 0 "D5" H 9250 3875 39  0000 C CNN
F 1 "LED_STD" H 9250 3700 30  0000 C CNN
F 2 "" H 9250 3800 60  0000 C CNN
F 3 "" H 9250 3800 60  0000 C CNN
	1    9250 3800
	0    1    1    0   
$EndComp
$Comp
L LED_STD D6
U 1 1 5383CB69
P 9500 3800
F 0 "D6" H 9500 3875 39  0000 C CNN
F 1 "LED_STD" H 9500 3700 30  0000 C CNN
F 2 "" H 9500 3800 60  0000 C CNN
F 3 "" H 9500 3800 60  0000 C CNN
	1    9500 3800
	0    1    1    0   
$EndComp
$Comp
L LED_STD D7
U 1 1 5383CBB0
P 9750 3800
F 0 "D7" H 9750 3875 39  0000 C CNN
F 1 "LED_STD" H 9750 3700 30  0000 C CNN
F 2 "" H 9750 3800 60  0000 C CNN
F 3 "" H 9750 3800 60  0000 C CNN
	1    9750 3800
	0    1    1    0   
$EndComp
$Comp
L BUZZER SP1
U 1 1 5383CD4E
P 9400 4550
F 0 "SP1" H 9500 4650 39  0000 C CNN
F 1 "BUZZER" H 9500 4325 39  0000 C CNN
F 2 "" H 9400 4550 60  0000 C CNN
F 3 "" H 9400 4550 60  0000 C CNN
	1    9400 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0102
U 1 1 5383D117
P 9000 3950
F 0 "#PWR0102" H 9000 3950 30  0001 C CNN
F 1 "GND" H 9000 3880 30  0001 C CNN
F 2 "" H 9000 3950 60  0000 C CNN
F 3 "" H 9000 3950 60  0000 C CNN
	1    9000 3950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0103
U 1 1 5383D14F
P 9250 3950
F 0 "#PWR0103" H 9250 3950 30  0001 C CNN
F 1 "GND" H 9250 3880 30  0001 C CNN
F 2 "" H 9250 3950 60  0000 C CNN
F 3 "" H 9250 3950 60  0000 C CNN
	1    9250 3950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0104
U 1 1 5383D187
P 9500 3950
F 0 "#PWR0104" H 9500 3950 30  0001 C CNN
F 1 "GND" H 9500 3880 30  0001 C CNN
F 2 "" H 9500 3950 60  0000 C CNN
F 3 "" H 9500 3950 60  0000 C CNN
	1    9500 3950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0105
U 1 1 5383D1BF
P 9750 3950
F 0 "#PWR0105" H 9750 3950 30  0001 C CNN
F 1 "GND" H 9750 3880 30  0001 C CNN
F 2 "" H 9750 3950 60  0000 C CNN
F 3 "" H 9750 3950 60  0000 C CNN
	1    9750 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 3600 9000 3700
Wire Wire Line
	9000 3900 9000 3950
Wire Wire Line
	9250 3900 9250 3950
Wire Wire Line
	9500 3900 9500 3950
Wire Wire Line
	9750 3900 9750 3950
Wire Wire Line
	8350 3350 9250 3350
Wire Wire Line
	9250 3350 9250 3400
Wire Wire Line
	9000 3350 9000 3400
Connection ~ 9000 3350
Wire Wire Line
	9250 3600 9250 3700
Wire Wire Line
	9500 3600 9500 3700
Wire Wire Line
	9750 3600 9750 3700
Wire Wire Line
	9750 3200 9750 3400
Wire Wire Line
	8350 3200 9750 3200
Wire Wire Line
	9500 3400 9500 3200
Connection ~ 9500 3200
Wire Wire Line
	9400 4550 8700 4550
Wire Wire Line
	2850 4650 3250 4650
Text Label 2850 2850 0    40   ~ 0
BUZ
Text Label 8700 4550 0    40   ~ 0
BUZ
$Comp
L GND #PWR0106
U 1 1 5383EF3B
P 9300 4700
F 0 "#PWR0106" H 9300 4700 30  0001 C CNN
F 1 "GND" H 9300 4630 30  0001 C CNN
F 2 "" H 9300 4700 60  0000 C CNN
F 3 "" H 9300 4700 60  0000 C CNN
	1    9300 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 4650 9300 4650
Wire Wire Line
	9300 4650 9300 4700
Text Notes 6100 3350 0    40   ~ 0
Timer 1 output 1 (PWM)
Text Notes 6100 3450 0    40   ~ 0
Timer 1 output 2 (PWM)
Text Notes 2100 2850 0    40   ~ 0
Timer 2 output 1 (PWM)
Wire Wire Line
	2850 2750 3250 2750
Wire Wire Line
	3250 2850 2850 2850
Wire Wire Line
	2850 2950 3250 2950
Wire Wire Line
	2850 3950 3250 3950
Text GLabel 5750 2550 2    40   Input ~ 0
AC_INT_XL
Wire Wire Line
	4950 2750 5750 2750
Wire Wire Line
	2850 4550 3250 4550
Wire Wire Line
	2750 3750 3250 3750
Wire Wire Line
	1800 4300 1800 3450
Wire Wire Line
	1150 4300 1800 4300
Wire Wire Line
	1900 4400 1900 3550
Wire Wire Line
	1150 4400 1900 4400
Wire Wire Line
	1250 4250 1250 4300
Connection ~ 1250 4300
Wire Wire Line
	1450 4250 1450 4400
Connection ~ 1450 4400
Wire Wire Line
	4950 3350 5750 3350
Wire Wire Line
	5750 4250 4950 4250
Wire Wire Line
	4950 2650 5750 2650
Wire Wire Line
	4950 2850 5750 2850
Wire Wire Line
	2400 3650 3250 3650
Wire Wire Line
	4950 3550 5750 3550
$Comp
L CRYSTAL_SMD X2
U 1 1 539B6F18
P 2600 3850
F 0 "X2" H 2600 3940 30  0000 C CNN
F 1 "48MHz" H 2630 3740 30  0000 L CNN
F 2 "" H 2600 3850 60  0000 C CNN
F 3 "" H 2600 3850 60  0000 C CNN
	1    2600 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	3250 2650 2850 2650
Wire Wire Line
	3250 4250 2850 4250
$Comp
L CAP C18
U 1 1 539B77D9
P 2300 3650
F 0 "C18" V 2250 3750 39  0000 C CNN
F 1 "10pF" V 2350 3750 39  0000 C CNN
F 2 "" H 2300 3650 60  0000 C CNN
F 3 "" H 2300 3650 60  0000 C CNN
	1    2300 3650
	-1   0    0    1   
$EndComp
$Comp
L CAP C24
U 1 1 539B7866
P 2300 4050
F 0 "C24" V 2250 4150 39  0000 C CNN
F 1 "10pF" V 2350 4150 39  0000 C CNN
F 2 "" H 2300 4050 60  0000 C CNN
F 3 "" H 2300 4050 60  0000 C CNN
	1    2300 4050
	-1   0    0    1   
$EndComp
Connection ~ 2600 3650
Wire Wire Line
	2750 4050 2750 3750
Wire Wire Line
	2400 4050 2750 4050
Connection ~ 2600 4050
$Comp
L GND #PWR0107
U 1 1 539B7F06
P 2100 4100
F 0 "#PWR0107" H 2100 4100 30  0001 C CNN
F 1 "GND" H 2100 4030 30  0001 C CNN
F 2 "" H 2100 4100 60  0000 C CNN
F 3 "" H 2100 4100 60  0000 C CNN
	1    2100 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0108
U 1 1 539B7F41
P 2100 3700
F 0 "#PWR0108" H 2100 3700 30  0001 C CNN
F 1 "GND" H 2100 3630 30  0001 C CNN
F 2 "" H 2100 3700 60  0000 C CNN
F 3 "" H 2100 3700 60  0000 C CNN
	1    2100 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0109
U 1 1 539B7F7C
P 2350 3900
F 0 "#PWR0109" H 2350 3900 30  0001 C CNN
F 1 "GND" H 2350 3830 30  0001 C CNN
F 2 "" H 2350 3900 60  0000 C CNN
F 3 "" H 2350 3900 60  0000 C CNN
	1    2350 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3850 2350 3850
Wire Wire Line
	2350 3850 2350 3900
Wire Wire Line
	2200 3650 2100 3650
Wire Wire Line
	2100 3650 2100 3700
Wire Wire Line
	2200 4050 2100 4050
Wire Wire Line
	2100 4050 2100 4100
NoConn ~ 3250 2450
NoConn ~ 3250 2550
NoConn ~ 3250 3050
NoConn ~ 4950 3050
NoConn ~ 4950 3150
NoConn ~ 4950 3250
$EndSCHEMATC
