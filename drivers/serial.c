#include "ports.h"

#define COM1 0x3f8

int init_serial() {
    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    //                  (hi byte)
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if(inb(COM1+0) != 0xAE) {
        return 1; // faulty port thingy
    }

    outb(COM1+4, 0x0F);
    return 0;
}

int serial_recieved() {
    return inb(COM1 + 5) & 1;
}

char read_serial() {
    while (serial_recieved() == 0);
    return inb(COM1);
}

int is_transmit_empty() {
    return inb(COM1 + 5) & 0x20;
}

void write_serial_char(char a) {
    while (is_transmit_empty() == 0);
    outb(COM1, a);
}

void serial_write(char* message) {
    int i = 0;
    while (message[i] != 0) {
        write_serial_char(message[i++]);
    }
}