####################################################################
# Makefile                                                         #
####################################################################

.SUFFIXES:				# ignore builtin rules
.PHONY: all debug release clean flash

####################################################################
# Definitions                                                      #
####################################################################

DEVICE = EFM32GG330F1024
PROJECTNAME_MAIN = usb_uart_bootloader
PROJECTNAME = usb_uart_bootloader_loader
# Name of interface configuration file used by OpenOCD
OOCD_IFACE ?= stlink-v2-1

OBJ_DIR = build
EXE_DIR = exe
LST_DIR = lst

####################################################################
# Definitions of toolchain.                                        #
# You might need to do changes to match your system setup          #
####################################################################

# Change path to the tools according to your system configuration
# DO NOT add trailing whitespace chars, they do matter !
WINDOWSCS  ?= GNU Tools ARM Embedded\4.7 2012q4
LINUXCS    ?= /opt/gcc-arm-none-eabi-4_8-2014q1

RMDIRS     := rm -rf
RMFILES    := rm -rf
ALLFILES   := /*.*
NULLDEVICE := /dev/null
SHELLNAMES := $(ComSpec)$(COMSPEC)

# HACK: Separately specifiy includes dir so we can keep it separate from the application.
#COMMONSW := C:/Users/xxxxxx/AppData/Roaming/energymicro
COMMONSW := ../../common

# Try autodetecting the environment
ifeq ($(SHELLNAMES),)
  # Assume we are making on a Linux platform
  TOOLDIR := $(LINUXCS)
else
  QUOTE :="
  ifneq ($(COMSPEC),)
    # Assume we are making on a mingw/msys/cygwin platform running on Windows
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    TOOLDIR := $(PROGRAMFILES)/$(WINDOWSCS)
    ifeq ($(findstring cygdrive,$(shell set)),)
      # We were not on a cygwin platform
      NULLDEVICE := NUL
    endif
  else
    # Assume we are making on a Windows platform
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    SHELL      := $(SHELLNAMES)
    TOOLDIR    := $(ProgramFiles)/$(WINDOWSCS)
    RMDIRS     := rd /s /q
    RMFILES    := del /s /q
    ALLFILES   := \*.*
    NULLDEVICE := NUL
  endif
endif

# Create directories and do a clean which is compatible with parallell make
$(shell mkdir $(OBJ_DIR)>$(NULLDEVICE) 2>&1)
$(shell mkdir $(EXE_DIR)>$(NULLDEVICE) 2>&1)
$(shell mkdir $(LST_DIR)>$(NULLDEVICE) 2>&1)
ifeq (clean,$(findstring clean, $(MAKECMDGOALS)))
  ifneq ($(filter $(MAKECMDGOALS),all debug release),)
    $(shell $(RMFILES) $(OBJ_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(EXE_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(LST_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
  endif
endif

CC      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-gcc$(QUOTE)
LD      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-ld$(QUOTE)
AR      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-ar$(QUOTE)
OBJCOPY = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-objcopy$(QUOTE)
DUMP    = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-objdump$(QUOTE)

####################################################################
# Flags                                                            #
####################################################################

# -MMD : Don't generate dependencies on system header files.
# -MP  : Add phony targets, useful when a h-file is removed from a project.
# -MF  : Specify a file to write the dependencies to.
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

#
# Add -Wa,-ahld=$(LST_DIR)/$(@F:.o=.lst) to CFLAGS to produce assembly list files
#
override CFLAGS += -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb \
-ffunction-sections \
-fdata-sections -fomit-frame-pointer       \
$(DEPFLAGS)

override ASMFLAGS += -x assembler-with-cpp -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb

#
# NOTE: The -Wl,--gc-sections flag may interfere with debugging using gdb.
#
#override LDFLAGS += -Xlinker -Map=$(LST_DIR)/$(PROJECTNAME).map -mcpu=cortex-m3 \
#-mthumb -T$(COMMONSW)/Device/EnergyMicro/EFM32GG/Source/G++/efm32gg.ld \
# -Wl,--gc-sections

#LOADER_LDFLAGS = $(LDFLAGS)

override LOADER_LDFLAGS += -Xlinker -Map=$(LST_DIR)/$(PROJECTNAME).map -mcpu=cortex-m3 \
-mthumb -T../efm32gg-rom.ld \
 -Wl,--gc-sections

LIBS = -Wl,--start-group -lgcc -lc -lnosys   -Wl,--end-group

INCLUDEPATHS += \
-I$(COMMONSW) \
-I$(COMMONSW)/CMSIS/Include \
-I$(COMMONSW)/Device/EnergyMicro/EFM32GG/Include \
-I$(COMMONSW)/emlib/inc \
-I$(COMMONSW)/usb/inc \
-I$(COMMONSW)/kits/common/drivers \
-I../


####################################################################
# Files                                                            #
####################################################################

C_SRC_COMMON =  \
$(COMMONSW)/emlib/src/em_system.c \
$(COMMONSW)/emlib/src/em_cmu.c \
$(COMMONSW)/emlib/src/em_assert.c \
$(COMMONSW)/emlib/src/em_gpio.c \
$(COMMONSW)/freewatch/freewatch_utils.c


C_SRC_MAIN += $(C_SRC_COMMON) \
$(COMMONSW)/Device/EnergyMicro/EFM32GG/Source/system_efm32gg.c \
$(COMMONSW)/emlib/src/em_emu.c \
$(COMMONSW)/emlib/src/em_dma.c \
$(COMMONSW)/emlib/src/em_rtc.c \
$(COMMONSW)/emlib/src/em_prs.c \
$(COMMONSW)/emlib/src/em_int.c \
$(COMMONSW)/emlib/src/em_timer.c \
$(COMMONSW)/emlib/src/em_usart.c \
$(COMMONSW)/kits/common/drivers/retargetio.c \
$(COMMONSW)/usb/src/em_usbd.c \
$(COMMONSW)/usb/src/em_usbdch9.c \
$(COMMONSW)/usb/src/em_usbhal.c \
$(COMMONSW)/usb/src/em_usbdep.c \
$(COMMONSW)/usb/src/em_usbdint.c \
$(COMMONSW)/usb/src/em_usbtimer.c \
../autobaud.c \
../boot.c \
../bootldio.c \
../cdc.c \
../crc.c \
../flash.c \
../main.c \
../retargetdebug.c \
../xmodem.c

C_SRC +=  $(C_SRC_COMMON) \
$(COMMONSW)/Device/EnergyMicro/EFM32GG/Source/system_efm32gg.c \
../boot.c \
../loader.c \




s_SRC += 

S_SRC +=  \
$(COMMONSW)/Device/EnergyMicro/EFM32GG/Source/GCC/startup_efm32gg.S

####################################################################
# Rules                                                            #
####################################################################

C_FILES_MAIN = $(notdir $(C_SRC_MAIN) )
C_FILES = $(notdir $(C_SRC) )
S_FILES = $(notdir $(S_SRC) $(s_SRC) )
#make list of source paths, sort also removes duplicates
C_PATHS_MAIN = $(sort $(dir $(C_SRC_MAIN) ) )
C_PATHS = $(sort $(dir $(C_SRC) ) )
S_PATHS = $(sort $(dir $(S_SRC) $(s_SRC) ) )

C_OBJS_MAIN = $(addprefix $(OBJ_DIR)/, $(C_FILES_MAIN:.c=.o))
C_OBJS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.o))
S_OBJS = $(if $(S_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.S=.o)))
s_OBJS = $(if $(s_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.s=.o)))
C_DEPS_MAIN = $(addprefix $(OBJ_DIR)/, $(C_FILES_MAIN:.c=.d))
C_DEPS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.d))
OBJS_MAIN = $(C_OBJS_MAIN) $(S_OBJS) $(s_OBJS)
OBJS = $(C_OBJS) $(S_OBJS) $(s_OBJS)

vpath %.c $(C_PATHS_MAIN):$(C_PATHS)
#vpath %.c_loader $(C_PATHS)
vpath %.s $(S_PATHS)
vpath %.S $(S_PATHS)

# Default build is debug build
all:      debug_bootloader

debug:      debug_bootloader

release:      release_bootloader

debug_bootloader:    CFLAGS += -DDEBUG -Os -g3 -ggdb
debug_bootloader:    $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin

release_bootloader:  CFLAGS += -DNDEBUG -Os
release_bootloader:  $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin

# Create objects from C SRC files
$(OBJ_DIR)/%.o: %.c
	@echo "Building file: $<"
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Create objects from C SRC files
$(OBJ_DIR)/%.o: %.c_loader
	@echo "Building file: $<"
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Assemble .s/.S files
$(OBJ_DIR)/%.o: %.s
	@echo "Assembling $<"
	$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.S
	@echo "Assembling $<"
	$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<


$(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.out: $(OBJS_MAIN)
	@echo "Linking target: $@"
	$(CC) $(LOADER_LDFLAGS) $(OBJS_MAIN) $(LIBS) -o $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.out

$(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin: $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.out
	@echo "Creating binary file"
	$(OBJCOPY) -O binary $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.out $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin
	@echo "Deleting loader.o to ensure it does not exist yet"
	$(RM) $(OBJ_DIR)\loader.o
	$(RM) ..\bootld.h
	@echo "DEBUGGING: Creating blob file needed by loader loader..."
	@touch $(PWD)/../bootld.h
	@echo "char bootloader[] = {" > $(PWD)/../bootld.h
	@hexdump -v -e '1/1 "  0x%02x, " 1/1 "0x%02x, " 1/1 "0x%02x, " 1/1 "0x%02x, " 1/1 "0x%02x, " 1/1 "0x%02x, " 1/1 "0x%02x, " 1/1 "0x%02x, \n"' $(PWD)/$(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin | sed "s/ 0x  ,//g" >> $(PWD)/../bootld.h
	@echo "};" >> $(PWD)/../bootld.h

clean:
ifeq ($(filter $(MAKECMDGOALS),all debug release),)
	$(RMDIRS) $(OBJ_DIR) $(LST_DIR) $(EXE_DIR)
endif

flashmain: $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin
	openocd -s $(COMMONSW)/openocd -f interface/$(OOCD_IFACE).cfg -f init.cfg -c "program $(EXE_DIR)/$(PROJECTNAME_MAIN)-rom.bin 0 verify reset"


# include auto-generated dependency files (explicit rules)
ifneq (clean,$(findstring clean, $(MAKECMDGOALS)))
-include $(C_DEPS)
endif
