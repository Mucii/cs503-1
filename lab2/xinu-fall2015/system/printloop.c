/*  printloop.c  - printloop */

#include <xinu.h>
#include <stdio.h>
#include <lab2a.h>

#define OUTERB 5
#define INNERB 100000000
void printloop(int val) {
  
  int i;
  long long j;
  for(i = 0; i <OUTERB; i++) {
	kprintf("%d", val);
	for(j = 0; j < INNERB; j++) {}
	//sleepms(2);
  }
}
