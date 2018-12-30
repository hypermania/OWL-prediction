program_NAME := main
dSFMT := dSFMT/dSFMT.c
program_C_SRCS := $(wildcard *.c) dSFMT/dSFMT.c
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_OBJS := $(program_C_OBJS)
CC := gcc

#LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir)) 
#LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library)) 
LDFLAGS += -ldl -pthread -msse4.2 -msse2 -mpopcnt -mbmi -mbmi2 -mavx -mavx2 -lm -O3 -ffast-math

CFLAGS += -std=c99

CFLAGS += -pthread -Wall

CFLAGS += -msse4.2 -msse2 -mpopcnt -mbmi -mbmi2 -mavx -mavx2 -lm

CFLAGS += -DDSFMT_MEXP=521

CFLAGS += -O3 -ffast-math



.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	$(RM) $(program_NAME)
	$(RM) $(program_OBJS)
	$(RM) $(wildcard *~)

distclean: clean

