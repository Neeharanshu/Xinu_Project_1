/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */
	/*Added by Neehar*/
	struct	procent	*prptr;		/* Ptr to process's table entry	*/
	uint32 cycles_high, cycles_low, cycles_high1, cycles_low1;
	unsigned long start, end;
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t""CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t"::: "%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
	prptr->sys_calls_count[4]+=1;
	/*Added by Neehar*/


	resched_cntl(DEFER_START);
	prptr = &proctab[currpid];
	
	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
		ready(dequeue(sleepq));
	}

	resched_cntl(DEFER_STOP);
	asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64)cycles_high << 32) | cycles_low );
	end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
	prptr->sys_calls_total[4] += (end - start);
	/*Added by Neehar*/
	return;
}
