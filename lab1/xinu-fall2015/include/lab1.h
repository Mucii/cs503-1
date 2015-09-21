#ifndef __LAB1_H_
#define __LAB1_H_

/* host2netl_asm.S */
long host2netl_asm(long param);
long net2hostl_asm(long param);

/* printsegaddress.c */
void printsegaddress(void);

/* printprocstks.c */
void printprocstks(void);

/* myprog.c */
int myprogA(char arg1, int arg2);
char myfuncA(char arg1, int arg2);

/* someprog.c */
void someprogA();
void someprogB();
int somefuncA(char);
int somefuncB(char);

/* rogue.c */
void rogueB();
void rogueB2();
void roguefuncB();
void roguefuncB2();

/* place any additional definitions here */
/* LAB1TODO */


#endif  // __LAB1_H_
