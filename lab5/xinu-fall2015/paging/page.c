#include <xinu.h>

/* All the pages related stuff
 * globaptinit() - 	initiliaze global page tables
 * getpdir() - 		get page directory and initialize it
 * freepdir() - 	free all page tables and free the page directory
 * geptable() - 	get page table and initialize it
 * freeptable() - 	free the page table
 */


#define DEVMEM 576


pt_t *globalpt[4] = {NULL, NULL, NULL, NULL};
pt_t *devpt = NULL;

int globalptinit() {

	int i, j;

	pt_t *pt;

	// first 4 page tables
	for(i = 0; i < 4; i++) {
		pt = getptable();
		if(pt == NULL) {
			return SYSERR;
		}
		for(j = 0; j < NENTRIES; j++) {

			pt[j].pt_pres 	= 1;
			pt[j].pt_write	= 1;
			pt[j].pt_user	= 0;
			pt[j].pt_pwt	= 0;
			pt[j].pt_pcd 	= 0;
			pt[j].pt_acc 	= 0;
			pt[j].pt_dirty 	= 0;
			pt[j].pt_mbz 	= 0;
			pt[j].pt_global = 0;
			pt[j].pt_avail 	= 1;

			/* identity mapping 
			 * we have seen i page tables and then (j - 1) in the current page table
			 */

			pt[j].pt_base 	= i * NENTRIES + j;
		}
		globalpt[i] = pt;
	}

	// dev memory page table
	pt = getptable();
	if(pt == NULL) {
		return SYSERR;
	}
	for(j = 0; j < NENTRIES; j++) {

		pt[j].pt_pres 	= 1;
		pt[j].pt_write	= 1;
		pt[j].pt_user	= 0;
		pt[j].pt_pwt	= 0;
		pt[j].pt_pcd 	= 0;
		pt[j].pt_acc 	= 0;
		pt[j].pt_dirty 	= 0;
		pt[j].pt_mbz 	= 0;
		pt[j].pt_global = 0;
		pt[j].pt_avail 	= 1;
		pt[j].pt_base 	= DEVMEM * NENTRIES + j;
	}

	devpt = pt;
	
	return OK;
}

pd_t *getpdir() {
	
	frame_t *frame;
	pd_t *pd;
	int i;
	frame = getframe();
	if(frame == NULL) {
		return NULL;
	}

	frame->type = FRAME_PD;

	pd = (pd_t *) FRAME2ADDR(frame->fid);
	
	// initialize page directory entries
	for(i = 0; i < NENTRIES; i++) {
		pd[i].pd_pres 	= 0;
		pd[i].pd_write	= 1;
		pd[i].pd_user	= 0;
		pd[i].pd_pwt	= 0;
		pd[i].pd_pcd 	= 0;
		pd[i].pd_acc 	= 0;
		pd[i].pd_mbz 	= 0;
		pd[i].pd_fmb 	= 0;
		pd[i].pd_global = 0;
		pd[i].pd_avail 	= 0;
		pd[i].pd_base 	= 0;
	}
	
	// add information for shared global page tables
	for(i = 0; i < 4; i++) {
		pd[i].pd_pres 	= 1;
		pd[i].pd_avail 	= 1;
		// TODO check whether this conversion will be needed or direct address
		pd[i].pd_base 	= VADDR2PNO((uint32) globalpt[i]);
		//kprintf("\nPID %d, GPT[%d] @ %d", currpid, i, pd[i].pd_base);

	}
	// TODO check whether this mapping is correct
	// add page table for dev memory page table
	pd[DEVMEM].pd_pres 	= 1;
	pd[DEVMEM].pd_avail 	= 1;
	pd[DEVMEM].pd_base 	= VADDR2PNO((uint32) devpt);
	//kprintf("\nPID %d, GPT[dev] @ %d", currpid, pd[DEVMEM].pd_base);
	return pd;
}

int freepdir(pd_t *pd) {
	// TODO
	
	kprintf("\nPID %d Free Dir called", currpid);	
	int i;

	for(i = 4; i < NENTRIES; i++) {
		
		if(pd[i].pd_pres && i != DEVMEM) {
			freeptable((pt_t *) PNO2VADDR(pd[i].pd_base));
		}
	}
	freeframe(ADDR2FRPTR(pd));
	return OK;	
}

pt_t *getptable() {

	frame_t *frame;
	pt_t *pt;
	int i;
	frame = getframe();
	if(frame == NULL) {
		return NULL;
	}

	frame->type = FRAME_PT;

	pt = (pt_t *) FRAME2ADDR(frame->fid);
	
	// initialize page table entries
	for(i = 0; i < NENTRIES; i++) {
		pt[i].pt_pres 	= 0;
		pt[i].pt_write	= 1;
		pt[i].pt_user	= 0;
		pt[i].pt_pwt	= 0;
		pt[i].pt_pcd 	= 0;
		pt[i].pt_acc 	= 0;
		pt[i].pt_dirty 	= 0;
		pt[i].pt_mbz 	= 0;
		pt[i].pt_global = 0;
		pt[i].pt_avail 	= 0;
		pt[i].pt_base 	= 0;
	}
	/* add hook */
	hook_ptable_create(frame->fid);
	return pt;
}

int freeptable(pt_t *pt) {

	kprintf("\nPID %d, freepagetable called for PT @ 0x%08x", currpid, pt);	

	freeframe(ADDR2FRPTR(pt));
	return OK;
}













