/* Code for myprogA and myfuncA */
#include <xinu.h>
#include <stdio.h>

#include <lab1.h>

unsigned long int *sp;

int myprogA(char arg1, int arg2) {

  /* this function calls myfuncA to conver arg1 to its uppercase
   * 1. value of arg1 is passed to myfuncA
   * 2. return value is 1 if conversion was successful else 0
   * 3. the stack contents are printed only if arg2 = 1;
   */

  if(arg2 == 1) {  

  	kprintf("\n-------------- in myprogA ------------");
  	asm("movl %esp, sp");
  	kprintf("\nStackAddr[%d] : %d", sp, *sp);

  	int i;
  	for(i = 1; i <= 12; i++) {
	 	kprintf("\nAddr[%d] : %d", (sp + i) , *(sp + i));
	}
  }

  char res = myfuncA(arg1, arg2);
  
  if(arg2 != 1) {
	sleep(1);
  }


  if(res <= 65 || res >= 90) return 0; 
  return 0;
}

char myfuncA(char arg1, int arg2) {
 
  /* the function converts the lowercase character [a-z] to an uppercase character if valid 
   * else returns return 0;
   */

  if(arg2 == 1) {
          
	  kprintf("\n-------------- in myfuncA ------------");
	  asm("movl %esp, sp");
	  kprintf("\nStackAddr[%d] : %d", sp, *sp);
          int i;
          for(i = 1; i <= 12; i++) {
                kprintf("\nAddr[%d] : %d", (sp + i) , *(sp + i));
	    }
  }

  if(arg2 != 1) {
  	sleep(1);
  }


  if(arg1 >= 97 && arg1 <= 122) {
  	char res = arg1 - 32;
  	//kprintf("in myfuncA: res = %c", res);
	return res;
  } else return 0;
}

