#include <xinu.h>

bsmap_t bsmap[MAX_BS_ENTRIES];
uint32 numfreebs;


int add_bsmapping(bsd_t bsid, pid32 pid, uint32 vpageno, uint32 npages) {

	if(bsmap[bsid].allocated == TRUE || bstab[bsid].isallocated == FALSE)
		return SYSERR;

	bsmap[bsid].pid = pid;
	bsmap[bsid].vpageno = vpageno;
	bsmap[bsid].npages = npages;
	bsmap[bsid].allocated = TRUE;
	numfreebs--;
	return OK;
}

int remove_bsmapping(bsd_t bsid) {
	
	
	if(bsmap[bsid].allocated == FALSE)
		return SYSERR;

	bsmap[bsid].pid = -1;
	bsmap[bsid].vpageno = 0;
	bsmap[bsid].npages = 0;
	bsmap[bsid].allocated = FALSE;
	numfreebs++;
	return OK;
}

int remove_bsmappings(pid32 pid) {

	int i;
	for(i = 0; i < MAX_BS_ENTRIES; i++) {

		if(bsmap[i].allocated == TRUE && bsmap[i].pid == pid) {
			
			if(deallocate_bs(i) == SYSERR)
				return SYSERR;
			if(remove_bsmapping(i) == SYSERR)
				return SYSERR;
		}
	}
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
	numfreebs = MAX_BS_ENTRIES;
	return OK;
}

void getbsmapping(pid32 pid, uint32 vpno, bsoff_t *bsoff) {

	if(isbadpid(pid)) {
		bsoff->bsid = -1;
		return;
	}

	
	int i;

	//kprintf("\nPID BSMP: pid %d vpno %d ", pid, vpno);
	for(i = 0; i < MAX_BS_ENTRIES; i++) {
		// check if map allocated to pid
		if(bsmap[i].pid == pid && bsmap[i].allocated == TRUE) {
			//kprintf("\nPID %d BSMP: bs %d start %d end %d", 
				//currpid, i, bsmap[i].vpageno, bsmap[i].vpageno + bsmap[i].npages);
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


