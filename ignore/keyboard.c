#include "keyboard.h"
#include "drivers/ports.h"
#include "cpu/isr.h"
#include "screen.h"
#include "libc/string.h"
#include "libc/function.h"
#include "kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define SHIFT 0x2A
#define SHIFT_UP 0xAA

static char key_buffer[256];

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const char sc_ascii_uppercase[] = {'?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

char shift_down = 0;
static void keyboard_callback(registers_t regs) {
	u8 scancode = inb(0x60);
	if (scancode == SHIFT) {
		shift_down = 1;
	}
	if (scancode == SHIFT_UP) {
		shift_down = 0;
	}
	if (scancode > SC_MAX) return;
	if(scancode == BACKSPACE) {
		if(!(strlen(key_buffer) > 0)) return;
		backspace(key_buffer);
		kprint_backspace();
	} else if (scancode == ENTER) {
		kprint("\n");
		//user_input(key_buffer);
		key_buffer[0] = '\0';
	} else {
		char letter;
		if (shift_down) {
			letter = sc_ascii_uppercase[(int)scancode];
		} else {
			letter = sc_ascii[(int)scancode];
		}
		char str[2] = {letter, '\0'};
		append(key_buffer, letter);
		kprint(str);
	}
	UNUSED(regs);
}

void init_keyboard() {
	register_interrupt_handler(IRQ1, keyboard_callback);
}
