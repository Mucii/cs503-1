/* lab3.h - CS503 15Fall, Lab 3 */

#ifndef __LAB3_H_
#define __LAB3_H_

#include <lab2b.h>

/** Emulating an RT application. */
void myperiodicrtapp(int rt_period, int rt_comp);

/** wait() with deadlock detection. */
syscall	waitd(sid32 sem);

/* LAB3TODO: Add more declarations here if necessary. */

#endif  // __LAB3_H_
