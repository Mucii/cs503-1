/*  main.c  - main file for CS 503, first lab */

#include <xinu.h>
#include <stdio.h>
#include <lab1.h>

unsigned long *sp, *ebp;

int test_host2netl_asm(long input, long expected_output) {
  long output = host2netl_asm(input);
  return (output == expected_output);
}

int test_net2hostl_asm(long input, long expected_output) {
  long output = net2hostl_asm(input);
  return (output == expected_output);
}


void test_runtimestack(void) {

	 kprintf("\n-------------- in Main ------------");
	 asm("movl %esp, sp");
	 kprintf("\nStackAddr[%d] : %d", sp, *sp);
         int i;
         for(i = 1; i <= 12; i++) {
 	        kprintf("\nAddr[%d] : %d", (sp + i) , *(sp + i));
	 }
	sleep(1);
	resume(create(myprogA, 2048, 20, "myprogA", 2, 'f', 1));
  
}

void test_procstks(void) {

	resume(create(myprogA, 2048, 25, "myprogA", 2, 'e', 0));
  	resume(create(myfuncA, 2048, 25, "myfuncA", 2, 'a', 0));
  	printprocstks();
}

void test_stack1(void) {

  	resume(create(someprogB, 2048, 20, "someprogB", 0, NULL));
  	resume(create(someprogA, 2048, 20, "someprogA", 0, NULL));
}

void test_rogue1(void) {
	resume(create(rogueB, 2048, 20, "rogueB", 0, NULL));
  	resume(create(someprogA, 2048, 20, "someprogA", 0, NULL));
}

void test_rogue2(void) {
        resume(create(rogueB2, 2048, 20, "rogueB2", 0, NULL));
	resume(create(someprogA, 2048, 20, "someprogA", 0, NULL));
	//sleepms(100);
	//printprocstks();
}


process main(void) {

  /* Assignment 3.1 */
  kprintf("\nTesting Lab1, assignment 3.1\n");
  if(!(test_host2netl_asm(0x12345678, 0x78563412))) {
    kprintf("host2netl_asm failed\n");
  } else {
    kprintf("host2netl_asm succeeded\n");
  }

  if(!(test_net2hostl_asm(0x12345678, 0x78563412))) {
      kprintf("net2hostl_asm failed\n");
  } else {
    kprintf("net2hostl_asm succeeded\n");
  }

  
  /* Assignment 3.2 */
  kprintf("\nTesting Lab1, assignment 3.2\n");
  printsegaddress();
  

  /* Assignment 3.3 */
  sleep(1); 
  kprintf("\nTesting Lab1, assignment 3.3\n");
  test_runtimestack();
  


  /* Assignment 3.4 */
  sleep(1);
  kprintf("\n\nTesting Lab1, assignment 3.4\n");  
  test_procstks();
  

  /* Assignment 5 */
  sleep(2);
  kprintf("\n\nTesting Lab1, assignment 5.1\n");
  test_stack1();
  
  
  
  kprintf("\n\nTesting Lab1, assignment 5.2\n\n");
  sleep(1);
  test_rogue1();
  
  
  /* Assignment 6 */
  //kprintf("\n\nTesting Lab1, assignment 6\n\n");
  //sleep(1);
  //test_rogue2();

  //kprintf("\nExiting Main :) ");
  return 0;

}
