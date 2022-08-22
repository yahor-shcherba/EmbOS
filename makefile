CC = cc

CFLAGS = \
	-Wall -Wextra -std=c11 -pedantic \
	-m32 \
	-ffreestanding \
	-fno-omit-frame-pointer \
	-fno-stack-protector \
	-fno-common \
	-nostdinc \
	-O2 -g \
	-I ./include -I ./sys

LDFLAGS = \
	-Xlinker -T ./sys/kernel.lds \
	-Xlinker --build-id=none \
	-m32 -static -nostdlib

SRC = \
	./sys/boot.S \
	./sys/main.c \
	./sys/uart.c

OBJ = $(patsubst %.S,%.o,$(patsubst %.c,%.o,$(SRC)))

BINARY = kernel

all: clean build

build: $(OBJ)
	$(CC) $(LDFLAGS) -o $(BINARY) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $^

qemu:
	qemu-system-i386 \
		-gdb tcp::1234 \
		-m 32 \
		-kernel $(BINARY)

clean:
	rm -rf $(OBJ) $(BINARY)
