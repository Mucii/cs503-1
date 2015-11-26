/* CS503 Fall 2015 - Lab 5 */

#ifndef __LAB5_H_
#define __LAB5_H_

// Hooks, in hooks.c
void hook_ptable_create(uint32 pagenum);
void hook_ptable_delete(uint32 pagenum);
void hook_pfault(void *addr);
void hook_pswap_out(uint32 pagenum, uint32 framenum);

/* Add more definitions here if necessary */

#endif // __LAB5_H_
