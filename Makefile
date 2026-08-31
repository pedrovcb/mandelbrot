CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -pthread -fopenmp
LDFLAGS = -pthread -fopenmp

SRCS = src/main.c \
       src/base.c \
       src/mandelbrotSerial.c \
	   src/mandelbrotOpenMP.c

OBJS    = $(SRCS:.c=.o)
TARGET  = mandelbrot

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c src/base.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f mandelbrot_pvcb_*.pgm times.txt