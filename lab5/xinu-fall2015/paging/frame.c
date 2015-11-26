#include <xinu.h>

frame_t frametab[NFRAMES];

frame_t *fifohead;

/* decrement reference count if page table and evict if free */

int decrementref(frame_t *frame) {

	if(frame->type != FRAME_PT)
		return SYSERR;
	frame->numref--;
	if(frame->numref == 0)
		freeframe(frame);

	return OK;
}

int freeframe(frame_t *free) {

	return SYSERR;
}


frame_t *fifoevict() {

	frame_t *prev, *cur;
	
	prev = NULL;
	cur = fifohead;

	// TODO check implementation
	while(cur) {
		if(cur->type == FRAME_BS) {
			prev->next = cur->next;
			return cur;
		}
		prev = cur;
		cur = cur->next;
	}

	return (frame_t*) NULL;
}


frame_t *evictframe() {

	int i = 0;
	frame_t *frame;

	while(i < NFRAMES) {
		frame = &frametab[i];
		if(frame->status == FRAME_FREE)
			return frame;
		i++;
	}

	/* evict according to policy */
	return fifoevict();
}

frame_t *getframe() {

	frame_t *frame, *iter;

	frame = evictframe();
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




