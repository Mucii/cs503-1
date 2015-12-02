/* pfisr.S - page fault isr */

/* calls the C funtion isroutine to handle the interrupt */
	.text
err:	.long	
	.globl pfisr, err

pfisr:
	cli
	popl err
	pushfl
	pushal
	call isroutine
	popal
	popfl
	sti
	iret
