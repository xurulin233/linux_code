PROG ?= ./example                 # Program we are building
PACK ?= ./pack                    # Packing executable
DELETE = rm -rf                   # Command to remove files
OUT ?= -o $(PROG)                 # Compiler argument for output file
SOURCES = main.c mongoose.c net.c # Source code files
CFLAGS = -W -Wall -Wextra -g -I.  # Build options


all: $(PROG)              # Default target. Build and run program

$(PROG): $(SOURCES)       # Build program from sources
	$(CC) $(SOURCES) $(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA) $(OUT)

clean:                    # Cleanup. Delete built program and all build artifacts
	$(DELETE) $(PROG) $(PACK) *.o *.obj
