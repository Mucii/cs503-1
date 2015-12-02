#include <xinu.h>

syscall isroutine() {

	/* handler for page faults 
	 * Page fault can occur only for virtual heap pages in following cases:
	 * 1. address not present
	 * 2. page table not present
	 * Note: In our case page directory is always present */

	intmask mask;
	struct procent *ptr;
	pd_t *pd;
	pt_t *pt;
	frame_t * frame;
	vaddr_t * addr;
	int pdi, pti;

	mask = disable();

	/* get faulted address */
	unsigned long a = getpfaddr();
	addr = (vaddr_t *) (&a);

	kprintf("\nPID %d ISR: Page fault for address 0x%08x ", currpid, a);
	ptr = &proctab[currpid];
	pd = ptr->pd;

	/* check whether valid mapping */
	bsoff_t bs;
	bs.bsid = -1; bs.offset = -1;
	getbsmapping(currpid, VADDR2PNO (a), &bs);
	if( isbadbsid(bs.bsid) ) {
		kprintf("\nPID %d ISR: Invalid page address 0x%08x ecountered. bsid = %d", 
				currpid, a, bs.bsid);
		kill(currpid);
		restore(mask);
		return SYSERR;
	}

	pdi = addr->pdindex;
	pti = addr->ptindex;

	/* if page table not present - create pt and fill entry */
	if( !pd[pdi].pd_pres ) {
		pt = getptable();
		if( pt == NULL) {

			kprintf("\nPID %d ISR: Page Table cannot be created. Aborting!", 
				currpid);
			kill(currpid);
			restore(mask);
			return SYSERR;

		}
		/* update page directory */
		pd[pdi].pd_pres 	= 1;
		pd[pdi].pd_write	= 1;
		pd[pdi].pd_user		= 0;
		pd[pdi].pd_pwt		= 0;
		pd[pdi].pd_pcd 		= 0;
		pd[pdi].pd_acc 		= 0;
		pd[pdi].pd_mbz 		= 0;
		pd[pdi].pd_fmb 		= 0;
		pd[pdi].pd_global 	= 0;
		pd[pdi].pd_avail 	= 0;
		pd[pdi].pd_base 	= VADDR2PNO((uint32) pt);
		kprintf("\nPID %d ISR: Page Table allocated @ 0x%08x page number: %d", 
			currpid, pt, VADDR2PNO(pt) );


	} else {
		pt = (pt_t *) PNO2VADDR(pd[pdi].pd_base);	
	}

	/* get a frame for the page */	
	frame = getframe();
	if (frame == NULL) {
		kprintf("\nPID %d ISR: Free Frame not available. Aborting!", 
				currpid);
		kill(currpid);
		restore(mask);
		return SYSERR;
	}
	kprintf("\nPID %d ISR: frame no %d @ 0x%08x allocated ", 
		currpid, frame->fid, FRAME2ADDR(frame->fid));

	frame->type = FRAME_BS;
	frame->vpagenum =  VADDR2PNO (a); 	/* faulted page num */
	frame->pid = currpid;
	/* read from backing store */
	if(bs.bsid != open_bs(bs.bsid)) {

		kprintf("\nPID %d ISR: Backing store cannot be opened!", 
				currpid);
		kill(currpid);
		restore(mask);
		return SYSERR;
	}
	
	 if( read_bs((char *)FRAME2ADDR(frame->fid), bs.bsid, bs.offset) == SYSERR) {
		kprintf("\nPID %d ISR: Backing store %d page no %d cannot be read!", 
				currpid, bs.bsid, bs.offset);
		kill(currpid);
		restore(mask);
		return SYSERR;
	 }

	/* update page table entry */
	pt[pti].pt_base  = FRAME2PGNO(frame->fid);
	pt[pti].pt_pres  = 1;
	pt[pti].pt_write = 1;

	/* increase numref for the frame of PT */
	frame = ADDR2FRPTR(pt);
	frame->numref++;

	/* set the pdbr */
	setPDBR(VADDR2PNO(pd));

	restore(mask);
	return OK;

}
