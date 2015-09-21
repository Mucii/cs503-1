/* Code for someprog[A|B] and somefunc[A|B] */
#include <xinu.h>
#include <stdio.h>

#include <lab1.h>

void someprogA(void) {
  
  kputc('A');

  somefuncA('a');
  
  //roguefuncB2();

  kputc('A');

  /* keep running for now */
  //long int i = 1000000000;
  //while(i-- > 0) {}
  
}

void someprogB(void) {
  
  kputc('B');

  somefuncB('b');

  kputc('B');
}

int somefuncA(char arg) {
  
  sleepms(QUANTUM * 15);
  kputc(arg);
  return 1;
}

int somefuncB(char arg) {
  
  sleepms(QUANTUM * 15);
  kputc(arg);
  return 1;

}

