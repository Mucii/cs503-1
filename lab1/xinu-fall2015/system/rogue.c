/* Code for rogue functions */
#include <xinu.h>
#include <stdio.h>

#include <lab1.h>

void rogueB(void) {
  /* LAB1TODO: write code for rogueB */

  kputc('B');

  sleepms(QUANTUM * 15);
  pid32 pid = getpid();

  roguefuncB(10, pid);

  kputc('B');
}

unsigned long int *sp;

void roguefuncB(int arg1, int arg2) {
  
    
  struct procent *proc = &proctab[arg2];

  if(currpid == arg2) {
  	asm("movl %esp, sp");
  } else sp = (unsigned long int *)proc->prstkptr;
  
  //kprintf("\nPID :%d  Name %s, Sptr: %d, Base: %d, Limit : %d", arg2, proc->prname, sp, proc->prstkbase, proc->prstkbase - proc->prstklen);
  
  if(sp < (unsigned long int *)(proc->prstkbase - 2 * proc->prstklen)) {
  	
	//kprintf("Stack overflow ");
	return;
  }

  roguefuncB(arg1, arg2);
}

void rogueB2(void) {
  
  //kputc('B');
  roguefuncB2();
  //pid32 pid = getpid();
  //struct procent *proc = &proctab[pid];
  //sleepms(QUANTUM * 15);

  long int i;
  for(i = 0; i < 100000000; i++);

  /* print stack of somefuncA asuuming it is created after this*/
 // printprocstks();

}


void roguefuncB2(void) {
  
  kputc('Q');
}
