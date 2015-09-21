/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab2b.h>

process	main(void)
{
  /* Lab2BTODO */
  kprintf("===== Lab2B =====\n");

  kprintf("===== Lab2B - Problem 3.1 =====\n");
  // All CPU intensive

  kprintf("===== Lab2B - Problem 3.2 =====\n");
  // All IO intensive

  kprintf("===== Lab2B - Problem 3.3 =====\n");
  // Half CPU-, half IO- intensive, v1

  kprintf("===== Lab2B - Problem 3.4 =====\n");
  // Half CPU-, half IO- intensive, v2

  kprintf("===== Lab2B - Bonus Problem =====\n");
  // One CPU-intensive, one hybrid

	return OK;
}
