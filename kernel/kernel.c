#include "drivers/screen.h"
#include "drivers/serial.h"
#include "libc/string.h";
#include "libc/mem.h"
#include "cpu/idt.h"
#include "common.h"

void printInt(long long);

int _start() {
    init_serial();
    clear_screen();
    init_descriptor_tables();
    kprint("mem: ");
    char *x;
    ll_to_ascii(memSize801(), x);
    kprint(x);
}

void printInt(long long in)
{
	if (in == 0){
		kprint("0");
		return;
	}
	char outputIintegers[21];
	
	// set end of string
	outputIintegers[20] = 0;
	// start here as index
	int index = 19;
	int modulus = 0;
	while (index >= 0)
	{
		modulus = mod(in, 10);
		

		outputIintegers[index] = "0123456789"[modulus];

		in -= modulus;
		// in = in / 10
		in = divm(in, 10);
		index--;
	}
	index = 0;
	// skip all the zeros

	while (outputIintegers[index] == '0')
	{
		index++;
	}
	kprint(&outputIintegers[index]);

	return;
}