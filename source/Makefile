#
# Makefile for DPT_balance_color
#

CC=g++
# if you want to use debugger, add -g to CFLAGS and LDFLAGS
CFLAGS=
LDFLAGS=-std=c++11 -O3
SOURCES=main.cpp shapes.cpp graphs.cpp windows.cpp calculate.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=DPT_balance_color
INCLUDES=shapes.h shape.h rectangle.h graph.h graphs.h window.h windows.h calculate.h

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
