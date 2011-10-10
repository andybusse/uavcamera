# FastAcc Makefile

# Target file name (without extension)
TARGET = fastacc

# Directories
SRCDIR = src
BUILDDIR = build

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# MCU name
MCU = atmega168p

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. You can then use this symbol in your source code to 
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
F_CPU = 16000000

# Note: prior to first use, program AVR's fuses as:
# avrdude -p atmega168p -P lpt1 -c stk200 -U lfuse:w:0xDF:m -U hfuse:w:0xD4:m
# for 16 MHz low-power crystal, with EEPROM memory preserved during reprogramming and brown-out detect at 4.3V (assumes powered by 5V)

# Generate list of C source files
SRC = $(wildcard $(SRCDIR)/*.c)

# Store C file names
SRCFILES = $(patsubst $(SRCDIR)/%,%,$(SRC))

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)UL

# Place -I options here
CINCS = -I../modules_common


#---------------- Compiler Options ----------------
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
CFLAGS = $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall
CFLAGS += $(CSTANDARD)

#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB = 
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# Module libraray for SkyCircuits payload modules
MODULES_COMMON_LIB = -lmodcom

# Place -L options here
LDINCS = -L../modules_common/build


#---------------- Linker Options ----------------
LDFLAGS = $(LDINCS) $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) $(MODULES_COMMON_LIB)


#---------------- Programming Options (avrdude) ----------------

# Programming hardware: alf avr910 avrisp bascom bsd 
# dt006 pavr picoweb pony-stk200 sp12 stk200 stk500
#
# Type: avrdude -c ?
# to get a full listing.
AVRDUDE_PROGRAMMER = stk200
#AVRDUDE_PROGRAMMER = ponyser

# com1 = serial port. Use lpt1 to connect to parallel port.
AVRDUDE_PORT = lpt1
#AVRDUDE_PORT = COM1

AVRDUDE_WRITE_FLASH = -U flash:w:$(BUILDDIR)/$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude> 
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)

#============================================================================


# Define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp
WINSHELL = cmd


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:


# Define all object files.
OBJ = $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.o))

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF $(BUILDDIR)/dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)


# Default target.
all: gccversion sizebefore build sizeafter

build: elf hex eep

elf: $(BUILDDIR)/$(TARGET).elf
hex: $(BUILDDIR)/$(TARGET).hex
eep: $(BUILDDIR)/$(TARGET).eep

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(BUILDDIR)/$(TARGET).hex
ELFSIZE = $(SIZE) -A $(BUILDDIR)/$(TARGET).elf
AVRMEM = avr-mem.sh $(BUILDDIR)/$(TARGET).elf $(MCU)

sizebefore:
	@if test -f $(BUILDDIR)/$(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	$(AVRMEM) 2>/dev/null; echo; fi

sizeafter:
	@if test -f $(BUILDDIR)/$(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	$(AVRMEM) 2>/dev/null; echo; fi


# Display compiler version information.
gccversion : 
	@$(CC) --version


# Program the device.  
program: $(BUILDDIR)/$(TARGET).hex $(BUILDDIR)/$(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)


# Create final output files (.hex, .eep) from ELF output file.
$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

$(BUILDDIR)/%.eep: $(BUILDDIR)/%.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@


# Link: create ELF output file from object files.
.SECONDARY : $(BUILDDIR)/$(TARGET).elf
.PRECIOUS : $(OBJ)
$(BUILDDIR)/%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
$(BUILDDIR)/%.s : $(SRCDIR)/%.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Target: clean project.
clean:
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(BUILDDIR)/$(TARGET).hex
	$(REMOVE) $(BUILDDIR)/$(TARGET).eep
	$(REMOVE) $(BUILDDIR)/$(TARGET).elf
	$(REMOVE) $(OBJ)
	$(REMOVE) $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.s))
	$(REMOVE) $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.d))
	$(REMOVE) $(BUILDDIR)/dep/*
	rmdir $(BUILDDIR)/dep

# Include the dependency files.
-include $(shell mkdir -p $(BUILDDIR)/dep 2>/dev/null) $(wildcard $(BUILDDIR)/dep/*)

# Listing of phony targets.
.PHONY : all sizebefore sizeafter gccversion build elf hex eep clean program

