#include <xinu.h>

bsmap_t bsmap[MAX_BS_ENTRIES];


int add_bsmapping(bsd_t bsid, pid32 pid, uint32 vpageno, uint32 npages) {

	if(bsmap[bsid].allocated == TRUE || bstab[bsid].isallocated == FALSE)
		return SYSERR;

	bsmap[bsid].pid = pid;
	bsmap[bsid].vpageno = vpageno;
	bsmap[bsid].npages = npages;
	bsmap[bsid].allocated = TRUE;
	return OK;
}

int remove_bsmapping(bsd_t bsid) {
	
	
	if(bsmap[bsid].allocated == FALSE)
		return SYSERR;

	bsmap[bsid].pid = -1;
	bsmap[bsid].vpageno = 0;
	bsmap[bsid].npages = 0;
	bsmap[bsid].allocated = FALSE;
	return OK;
}

int init_bsmap() {

	int i;
	
	for(i = 0; i < MAX_BS_ENTRIES; i++) { 
		bsmap[i].pid = -1;
		bsmap[i].vpageno = 0;
		bsmap[i].npages = 0;
		bsmap[i].allocated = FALSE;
	}
	
	return OK;
}

void getbsmapping(pid32 pid, uint32 vpno, bsoff_t *bsoff) {

	if(isbadpid(pid)) {
		bsoff->bsid = -1;
		return;
	}

	int i;
	for(i = 0; i < MAX_BS_ENTRIES; i++) {
		// check if map allocated to pid
		if(bsmap[i].pid == pid && bsmap[i].allocated == TRUE) {
			if(validbspage(i, vpno)) {
				
				bsoff->bsid = i;
				bsoff->offset = (vpno - bsmap[i].vpageno);
				return;
			}		
		}
	}
	bsoff->bsid = -1;
	return;
}


