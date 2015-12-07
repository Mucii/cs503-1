#include <xinu.h>

unsigned long var;

unsigned long readcr0() {

	intmask mask;
	unsigned long temp;
	mask = disable();

	asm("pushl %eax");
	asm("movl %cr0, %eax");
	asm("movl %eax, var");
	asm("popl %eax");

	temp = var;
	restore(mask);
	return temp;
}

unsigned long getpfaddr() {

	intmask mask;
	unsigned long temp;
	mask = disable();

	asm("pushl %eax");
	asm("movl %cr2, %eax");
	asm("movl %eax, var");
	asm("popl %eax");

	temp = var;
	restore(mask);
	return temp;
}
void writecr0(unsigned long n) {

	intmask mask;
	mask = disable();

	var = n;
	asm("pushl %eax");
	asm("movl var, %eax");
	asm("movl %eax, %cr0");
	asm("popl %eax");

	restore(mask);
}


void writecr3(unsigned long n) {

	intmask mask;
	mask = disable();

	var = n;
	asm("pushl %eax");
	asm("movl var, %eax");
	asm("movl %eax, %cr3");
	asm("popl %eax");

	restore(mask);
}

void enablepaging() {

	// write to PG bit
	unsigned long temp = readcr0();
	temp |= (0x1 << 31);
	writecr0(temp);

}

void setPDBR(unsigned long addr) {

	// shift by 12 to write addr to upper 20 MSB
	addr = addr << 12;
	writecr3(addr);
}


