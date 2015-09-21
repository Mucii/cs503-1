/* File used to implement printprocstks() */
#include <xinu.h>
#include <stdio.h>

#include <lab1.h>

unsigned long int *esp;

void printprocstks(void) {
 

 int piterator;
 for(piterator = 0; piterator < NPROC; piterator++) {
 	
	if(piterator != 0 && isbadpid(piterator))
		continue;

	unsigned long int *sp;
	struct procent *proc = &proctab[piterator];

	if(piterator == currpid) {
		asm("movl %esp, esp");
		sp = esp;
		
	} else {
		
		sp = (unsigned long int*)proc->prstkptr;
	}
	
	kprintf("\nProcess [ %s ]",proc->prname);
	kprintf("\tPid: %d", piterator);
	kprintf("\nStack:  Base : %d",proc->prstkbase);
	kprintf("\n\tLen : %d",proc->prstklen);
	kprintf("\n\tLimit : %d", (proc->prstkbase - proc->prstklen));
	kprintf("\n\tStkptr: %d", sp);

	/*
	if(piterator > 3) {
		kprintf("\n\nPrinting process %s's stack", proc->prname);
		unsigned long int *ptr = (unsigned long int*)proc->prstkbase;
		for(; ptr >= sp ; ptr--)
			kprintf("\nAddr[%d] : %d", ptr, *(ptr));
	}*/
 }
}
