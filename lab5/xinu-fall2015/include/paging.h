/* paging.h */

#ifndef __PAGING_H_
#define __PAGING_H_

/* Structure for a page directory entry */

typedef struct {
	unsigned int pd_pres	: 1;		/* page table present?		*/
	unsigned int pd_write : 1;		/* page is writable?		*/
	unsigned int pd_user	: 1;		/* is use level protection?	*/
	unsigned int pd_pwt	: 1;		/* write through cachine for pt? */
	unsigned int pd_pcd	: 1;		/* cache disable for this pt?	*/
	unsigned int pd_acc	: 1;		/* page table was accessed?	*/
	unsigned int pd_mbz	: 1;		/* must be zero			*/
	unsigned int pd_fmb	: 1;		/* four MB pages?		*/
	unsigned int pd_global: 1;		/* global (ignored)		*/
	unsigned int pd_avail : 3;		/* for programmer's use		*/
	unsigned int pd_base	: 20;		/* location of page table?	*/
} pd_t;

/* Structure for a page table entry */

typedef struct {
	unsigned int pt_pres	: 1;		/* page is present?		*/
	unsigned int pt_write : 1;		/* page is writable?		*/
	unsigned int pt_user	: 1;		/* is use level protection?	*/
	unsigned int pt_pwt	: 1;		/* write through for this page? */
	unsigned int pt_pcd	: 1;		/* cache disable for this page? */
	unsigned int pt_acc	: 1;		/* page was accessed?		*/
	unsigned int pt_dirty : 1;		/* page was written?		*/
	unsigned int pt_mbz	: 1;		/* must be zero			*/
	unsigned int pt_global: 1;		/* should be zero in 586	*/
	unsigned int pt_avail : 3;		/* for programmer's use		*/
	unsigned int pt_base	: 20;		/* location of page?		*/
} pt_t;


#define NBPG		4096	/* number of bytes per page	*/
#define FRAME0		1024	/* zero-th frame		*/

#ifndef NFRAMES
#define NFRAMES		3072	/* number of frames		*/
#endif

#define MAP_SHARED 1
#define MAP_PRIVATE 2

#define FIFO 3
#define GCA 4

#define MAX_ID		7		/* You get 8 mappings, 0 - 7 */
#define MIN_ID		0


/* page directories related definition */
extern pt_t *globalpt[4];
int ptinit();
pd_t * getpdir();
int freepdir(pd_t *pd);
pt_t * getptable();
int freeptable(pt_t *pt);



/* Frame and Inverted page table related definitions */
#define FRAME_FREE	1
#define FRAME_USED	0

#define FRAME_PD	2
#define FRAME_PT	3
#define FRAME_BS	4

typedef struct _frame {
	uint32		fid;		/* frame index */
	uint32		status;		/* used or not */
	uint32		type;		/* shared or private */
	uint32 		dirty;		/* whether frame is dirty */
	uint32 		numref;		/* number of pages mapped if FRAME_PT */
	struct _frame 	*next;		/* fifo: next frame allocated */
	pid32		pid;		/* process whos page is mapped */
	uint32		vpagenum;	/* virtual page num */
	bsd_t		bsid;		/* id of backing store */
	uint32		bspagenum;	/* page num within backing store */
} frame_t;

extern frame_t frametab[NFRAMES];

int frameinit();
frame_t *getframe();
int freeframe(frame_t *frame);
int decrementref(frame_t *frame);

#endif // __PAGING_H_





















