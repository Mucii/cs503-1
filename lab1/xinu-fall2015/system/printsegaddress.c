/* File used to implement printsegaddress() */
#include <xinu.h>
#include <stdio.h>

#include <lab1.h>

void printsegaddress(void) {

	/* The print format for the 12 bytes before and after an address : addr is as follow
	 * addr-3 addr-2 addr-1 addr+1 addr+2 addr+3
	 */

  	kprintf("\nEnd of Text Segment: \nAddr : 0x%08x  \nContent : 0x%08x \nPreceding 12 bytes : 0x%08x %08x %08x \nFollowing 12 bytes : 0x%08x %08x %08x \n", &etext, *(&etext), *((int *)&etext - 3), *((int *)&etext - 2), *((int *)&etext - 1), *((int *)&etext + 1), *((int *)&etext + 2), *((int *)&etext + 3));

	kprintf("\nEnd of Data Segment: \nAddr : 0x%08x  \nContent : 0x%08x \nPreceding 12 bytes : 0x%08x %08x %08x \nFollowing 12 bytes : 0x%08x %08x %08x \n", &edata, *(&edata), *((int *)&edata - 3), *((int *)&edata - 2), *((int *)&edata - 1), *((int *)&edata + 1), *((int *)&edata + 2), *((int *)&edata + 3));

	kprintf("\nEnd of BSS Segment: \nAddr : 0x%08x  \nContent : 0x%08x \nPreceding 12 bytes : 0x%08x \t 0x%08x \t 0x%08x \nFollowing 12 bytes : 0x%08x \t 0x%08x \t 0x%08x \n", &end, *(&end), *((int *)&end - 3), *((int *)&end - 2), *((int *)&end - 1), *((int *)&end + 1), *((int *)&end + 2), *((int *)&end + 3));
}
