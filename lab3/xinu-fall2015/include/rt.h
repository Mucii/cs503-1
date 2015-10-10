/* rt.h - CS503 15Fall, Lab 3 */

#ifndef __RT_H_
#define __RT_H_

pid32	rt_create(int rt_period, int rt_comp, void *funcaddr, uint32 ssize, char *name, uint32 nargs, ...);

#endif  // __RT_H_
