/* yield.c - yield */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{
	/*Added by Neehar*/
	/*Added by Neehar*/
	struct	procent	*prptr;		/* Pointer to proc. table entry */
	prptr = &proctab[currpid];
	
	uint32 cycles_high, cycles_low, cycles_high1, cycles_low1;
	unsigned long start, end;
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t""CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t"::: "%rax", "%rbx", "%rcx", "%rdx");
	/*Added by Neehar*/
	intmask	mask;			/* Saved interrupt mask		*/
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
	/*Added by Neehar*/

	mask = disable();
	resched();
	/*Added by Neehar*/
	prptr->sys_calls_count[1]+=1;
	asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64)cycles_high << 32) | cycles_low );
	end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
	prptr->sys_calls_total[1] += (end - start);
	/*Added by Neehar*/
	restore(mask);
	return OK;
}
