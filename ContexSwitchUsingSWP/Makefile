##############################################################################################
#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!
#
##############################################################################################
#
# On command line:
#
# make all = Create project
#
# make clean = Clean project files.
#
# To rebuild project do "make clean" and "make all".
#

##############################################################################################
# Start of default section
#
# Toolchain for Olimex OpenOCD Development Suite
OCDTOOLCHAIN =

# Toolchain for original arm-elf Yagarto tools
# Add your Yagarto bin to your system path: C:\OlimexODS\yagarto\bin;
TOOLCHAINDIR=/opt/arm-none-eabi/bin
TOOLCHAIN  =arm-none-eabi-

CC          = $(TOOLCHAINDIR)/$(TOOLCHAIN)g++
# AS          = $(TOOLCHAINDIR)/$(TOOLCHAIN)-gcc -x assembler-with-cpp
AS          = $(TOOLCHAINDIR)/$(TOOLCHAIN)as
LD          = $(TOOLCHAINDIR)/$(TOOLCHAIN)gcc
OBJCOPY     = $(TOOLCHAINDIR)/$(TOOLCHAIN)objcopy

# Yagarto's arm-none-eabi-objdump has an issue when run from MAKE. So use the objdump
# from /yagarto/arm-none-eabi/bin (e.g., C:\OlimexODS\yagarto\arm-none-eabi\bin)
#OBJDUMP     = $(TOOLCHAIN)-objdump
OBJDUMP     = $(TOOLCHAINDIR)/$(TOOLCHAIN)objdump
AR          = $(TOOLCHAINDIR)/$(TOOLCHAIN)ar
RANLIB      = $(TOOLCHAINDIR)/$(TOOLCHAIN)ranlib

MCU  = arm7tdmi

# List command line functions
RM = rm -fr

# List all default C defines here, like -D_DEBUG=1
WARNINGS = -Wall -Wextra -Weffc++
DDEFS = -DDEBUG=1 -g3 $(WARNINGS)

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

# Define project name here
PROJECT = ContexSwitchUsingSWP

# Define linker script file here
#LDSCRIPT= ./prj/lpc2378_ram.ld
LDSCRIPT= ./bsp/lpc2378_flash.ld

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List C source files here
SRC  = \
	./main/main.cpp \
	./dbg/print.cpp \
	./dbg/debugger.cpp \
	./main/mini_cpp.cpp \
	./bsp/bsp.cpp

# List ASM source files here
ASRC = ./bsp/startup.S

# List all user directories here
UINCDIR = \
	./inc \
	./main \
	./dbg \
	./bsp \
	./prj

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

# Define optimisation level here -O0 -O1 -O2 -Os -03
OPT = -O0

#
# End of user defines
##############################################################################################


INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))
DEFS    = $(DDEFS) $(UDEFS)
ADEFS   = $(DADEFS) $(UADEFS)
OBJS    = $(SRC:.cpp=.o) $(ASRC:.S=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mcpu=$(MCU)

ASFLAGS = $(MCFLAGS) -ahm -mapcs-32
#ODFLAGS	= -x --syms -S
CPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -fno-rtti -fno-exceptions -fmessage-length=0 -mthumb-interwork -mlong-calls -fomit-frame-pointer -fverbose-asm $(DEFS)
LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections $(LIBDIR)

# Generate dependency information
CPFLAGS += -MMD -MP -MF .dep/$(@F).d

#
# makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin $(PROJECT).lst
	$(MAKE) -C tests all

%.o : %.cpp
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

%.o : %.S
	$(AS) -c $(ASFLAGS) $< -o $@

%elf: $(OBJS)
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $(OBJS)

%lst: %elf
	$(OBJDUMP) -dSst $< > $@

%bin: %elf
	$(OBJCOPY) -O binary -S $< $@

%srec: %elf
	$(OBJCOPY) -O srec -S $< $@

%hex: %elf
	$(OBJCOPY) -O ihex -S $< $@

.PHONY: clean
clean:
	$(RM) tests/RunAllTests.sh
	$(RM) $(OBJS)
	$(RM) $(PROJECT).elf
	$(RM) $(PROJECT).map
	$(RM) $(PROJECT).hex
	$(RM) $(PROJECT).bin
	$(RM) $(PROJECT).srec
	$(RM) $(PROJECT).lst
	$(RM) $(SRC:.cpp=.cpp.bak)
	$(RM) $(SRC:.cpp=.lst)
	$(RM) $(ASRC:.S=.S.bak)
	$(RM) $(ASRC:.S=.lst)
	$(RM) .dep

.PHONY: gdb
gdb:
	openocd -f /usr/share/openocd/scripts/target/lpc2378.cfg -f /usr/share/openocd/scripts/interface/olimex-arm-usb-ocd.cfg -d2

## Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
