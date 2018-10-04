/* wait.c - wait */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wait  -  Cause current process to wait on a semaphore
 *------------------------------------------------------------------------
 */
syscall	wait(
	  sid32		sem		/* Semaphore on which to wait  */
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/
/*Added by Neehar*/
	uint32 cycles_high, cycles_low, cycles_high1, cycles_low1;
	unsigned long start, end;
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t""CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t"::: "%rax", "%rbx", "%rcx", "%rdx");

	mask = disable();
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
	prptr = &proctab[currpid];
	if (isbadsem(sem)) {
		prptr->sys_calls_count[9]+=1;
		asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
		start = ( ((uint64)cycles_high << 32) | cycles_low );
		end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
		prptr->sys_calls_total[9] += (end - start);
		restore(mask);
		return SYSERR;
	}

	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
	prptr->sys_calls_count[9]+=1;
	asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64)cycles_high << 32) | cycles_low );
	end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
	prptr->sys_calls_total[9] += (end - start);
		restore(mask);
		return SYSERR;
	}

	if (--(semptr->scount) < 0) {		/* If caller must block	*/
		prptr = &proctab[currpid];
		prptr->prstate = PR_WAIT;	/* Set state to waiting	*/
		prptr->prsem = sem;		/* Record semaphore ID	*/
		enqueue(currpid,semptr->squeue);/* Enqueue on semaphore	*/
		resched();			/*   and reschedule	*/
	}
	prptr->sys_calls_count[9]+=1;
	asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
	start = ( ((uint64)cycles_high << 32) | cycles_low );
	end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
	prptr->sys_calls_total[9] += (end - start);

	restore(mask);
	return OK;
}
