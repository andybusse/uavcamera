# MCU name
MCU = atmega168p

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. You can then use this symbol in your source code to 
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
F_CPU = 16000000

# Target file name (without extension).
TARGET = libmodcom

# Directories
SRCDIR = mod
BUILDDIR = build

# List C source files here. (C dependencies are automatically generated.)
SRC = $(wildcard $(SRCDIR)/*.c)

# Just store C file names
SRCFILES = $(patsubst $(SRCDIR)/%,%,$(SRC))

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = 

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)UL

# Place -I options here
CINCS =


#---------------- Compiler Options ----------------
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
CFLAGS = $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

#---------------- Archive Options ----------------
ARFLAGS = rcs

# Define programs and commands.
CC = avr-gcc
AR = avr-ar
REMOVE = rm -f

# Define all object files.
OBJ = $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.o))

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF $(BUILDDIR)/dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)

# Default target.
all: build

build: a

a: $(BUILDDIR)/$(TARGET).a

# Link: create an archive (.a) output file from object files.
.SECONDARY : $(BUILDDIR)/$(TARGET).a
.PRECIOUS : $(OBJ)
$(BUILDDIR)/%.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

# Compile: create object files from C source files.
$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Target: clean project.
clean:
	$(REMOVE) $(BUILDDIR)/$(TARGET).a
	$(REMOVE) $(OBJ)
	$(REMOVE) $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.s))
	$(REMOVE) $(addprefix $(BUILDDIR)/,$(SRCFILES:.c=.d))
	$(REMOVE) $(BUILDDIR)/dep/*
	rmdir $(BUILDDIR)/dep

# Include the dependency files.
-include $(shell mkdir -p $(BUILDDIR)/dep 2>/dev/null) $(wildcard $(BUILDDIR)/dep/*)

# Listing of phony targets.
.PHONY : all build a clean
