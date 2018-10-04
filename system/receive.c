/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
/*Added by Neehar*/
	uint32 cycles_high, cycles_low, cycles_high1, cycles_low1;
	unsigned long start, end;
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t""CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t"::: "%rax", "%rbx", "%rcx", "%rdx");

	mask = disable();
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
	/*Added by Neehar*/
	prptr = &proctab[currpid];
	prptr->sys_calls_count[5]+=1;
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64)cycles_high << 32) | cycles_low );
	end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
	prptr->sys_calls_total[5] += (end - start);
	restore(mask);
	return msg;
}
