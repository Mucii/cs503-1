#include <xinu.h>

frame_t frametab[NFRAMES];

frame_t *fifohead;

/* decrement reference count if page table and remove if free */

int decrementref(frame_t *frame) {

	if(frame->type != FRAME_PT)
		return SYSERR;
	frame->numref--;
	if(frame->numref == 0)
		freeframe(frame);

	return OK;
}

int isdirty(frame_t *frame) {

	/* no checks - internal function */
	pd_t *pd;
	pt_t *pt;
	int dirty = 0, i, j;


	if(isbadpid(frame->pid)) return dirty;
	
	pd = proctab[frame->pid].pd;
	uint32 addr = FRAME2ADDR(frame->fid);
	
	for(i = 4; i < NENTRIES; i++) {
		
		if(pd[i].pd_pres) {

			pt = (pt_t *) PNO2VADDR(pd[i].pd_base);

			// search page in the page table
			for(j = 0; j < NENTRIES; j++) {

				if(pt[j].pt_pres && addr == pt[j].pt_base) {
					
					// found page entry
					dirty = pt[j].pt_dirty;

					// zero out the page
					pt[j].pt_pres 	= 0;
					pt[j].pt_write	= 0;
					pt[j].pt_user	= 0;
					pt[j].pt_pwt	= 0;
					pt[j].pt_pcd 	= 0;
					pt[j].pt_acc 	= 0;
					pt[j].pt_dirty 	= 0;
					pt[j].pt_mbz 	= 0;
					pt[j].pt_global = 0;
					pt[j].pt_avail 	= 0;
					pt[j].pt_base 	= 0;
					
					// decrease reference count of corresponding PT
					frame_t *fr = ADDR2FRPTR(pt);
					decrementref(fr);

					// this frame might get freed as a result
					// if freed pd needs to be updated
					if(fr->status == FRAME_FREE)
						pd[i].pd_pres = 0;

					return dirty;
				}
			}
		}
	}

	return dirty;
}

// remove all frames in the list which have status = FRAME_FREE
void cleanupfifo() {

	frame_t *cur, *prev;

	prev = NULL;
	cur = fifohead;
	
	while(cur != NULL) {
		if(cur->status == FRAME_FREE) {
			if(cur == fifohead) {
				fifohead = fifohead->next;
			} else {
				prev->next = cur->next;
			}
			cur = cur->next;
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
}


int freeframe(frame_t *frame) {

	if(frame == NULL)
		return OK;

	kprintf("\nPID %d, Free frame called for frame 0x%08x", currpid, FRAME2ADDR(frame->fid));
	
	// write_back if a dirty frame mapped to backing store
	if(frame->type == FRAME_BS && isdirty(frame)) {

		bsoff_t bo;
		getbsmapping(frame->pid, frame->vpagenum, &bo);
		if(isbadbsid(bo.bsid))
			return SYSERR;

		if(open_bs(bo.bsid) == bo.bsid) 
			write_bs((char *) FRAME2ADDR(frame->fid), bo.bsid, bo.offset);
		close_bs(bo.bsid);
	}

	// update frame entry
	frame->status 	= FRAME_FREE;
	frame->type 	= FRAME_FREE;
	frame->numref 	= 0;
	frame->dirty 	= 0;
	frame->bsid	= -1;
	frame->bspagenum= 0;
	frame->pid	= -1;
	frame->vpagenum	= 0;

	/* remove frame from fifolist currently 
	 * have to edit for bonus problem TODO */
	cleanupfifo();
	
	frame->next = NULL;

	return OK;
}

frame_t *fiforeplace() {

	kprintf("\nFrame replacement called..");
	frame_t *prev, *cur;
	
	prev = NULL;
	cur = fifohead;

	// TODO check implementation
	// TODO whether write back page here
	while(cur) {
		if(cur->type == FRAME_BS) {	
			
			// free frame
			freeframe(cur);
			return cur;
		}
		prev = prev->next;
		cur = cur->next;
	}

	return (frame_t*) NULL;
}


frame_t *removeframe() {

	int i = 0;
	frame_t *frame;
	kprintf("\nPID %d Frame removal called..", currpid);
	while(i < NFRAMES) {
		frame = &frametab[i];
		if(frame->status == FRAME_FREE)
			return frame;
		i++;
	}

	/* remove according to policy */
	return fiforeplace();
}

frame_t *getframe() {

	frame_t *frame, *iter;
	kprintf("\nPID %d Get Frame called..", currpid);
	frame = removeframe();
	if(frame == NULL) {
		return (frame_t *) NULL;
	}
	
	// update frame entry
	frame->status 	= FRAME_USED;
	frame->numref 	= 0;
	frame->dirty 	= 0;
	frame->bsid	= -1;
	frame->bspagenum= 0;
	frame->pid	= currpid;
	frame->vpagenum	= 0;
	frame->next	= NULL;

	// update fifo list
	if(fifohead == NULL) {
		fifohead  = frame;
	} else {
		iter = fifohead;
		while(iter->next != NULL)
			iter = iter->next;
		iter->next = frame;
	}
	return frame;

}

int frameinit() {

	int i;
	
	fifohead = NULL;
	for(i = 0; i < NFRAMES; i++) {

		frametab[i].fid = i;
		frametab[i].status = FRAME_FREE;
		frametab[i].type = FRAME_FREE;
		frametab[i].dirty = 0;
		frametab[i].numref = 0;
		frametab[i].next = NULL;
		frametab[i].pid = -1;
		frametab[i].vpagenum = 0;
		frametab[i].bsid = -1;
		frametab[i].bspagenum = 0;
	}
	return OK;
}


syscall cleanslate() {

	if(isbadpid(currpid)) 
		return SYSERR;

	struct procent *ptr;
	int i;
	kprintf("\nPID %d Clean Slate Called ", currpid);
	ptr = &proctab[currpid];

	// remove all frames which are mapped to current process
	// write back to backing store if required
	for(i = 0; i < NFRAMES; i++) {

		if(frametab[i].pid == currpid && frametab[i].type == FRAME_BS) {
			//freeframe(&frametab[i]);
		}	
	}	

	// remove all backing store mappings and release them
	for(i = 0; i < MAX_BS_ENTRIES; i++) {

		if(bsmap[i].allocated == TRUE && bsmap[i].pid == currpid) {
			
			deallocate_bs(i);
			remove_bsmapping(i);
		}
	}
	
	// remove the page directory and page table information
	freepdir(ptr->pd);	
	
	return OK;	
}








