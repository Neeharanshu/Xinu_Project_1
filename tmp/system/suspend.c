/* suspend.c - suspend */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  suspend  -  Suspend a process, placing it in hibernation
 *------------------------------------------------------------------------
 */
syscall	suspend(
	  pid32		pid		/* ID of process to suspend	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	prio;			/* Priority to return		*/
	/*Added by Neehar*/
	uint32 cycles_high, cycles_low, cycles_high1, cycles_low1;
	unsigned long start, end;
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t""CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");
	asm volatile ("CPUID\n\t""RDTSC\n\t"::: "%rax", "%rbx", "%rcx", "%rdx");
	mask = disable();
	asm volatile ("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high), "=r"(cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
	/*Added by Neehar*/
	if (isbadpid(pid) || (pid == NULLPROC)) {
		restore(mask);
		return SYSERR;
	}

	/* Only suspend a process that is current or ready */

	prptr = &proctab[pid];
	if ((prptr->prstate != PR_CURR) && (prptr->prstate != PR_READY)) {
		prptr->sys_calls_count[3]+=1;
		/*Added by Neehar*/
		asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
		start = ( ((uint64)cycles_high << 32) | cycles_low );
		end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
		prptr->sys_calls_total[3] += (end - start);
		/*Added by Neehar*/
		restore(mask);
		return SYSERR;
	}
	if (prptr->prstate == PR_READY) {
		getitem(pid);		    /* Remove a ready process	*/
					    /*   from the ready list	*/
		prptr->prstate = PR_SUSP;
	} else {
		prptr->prstate = PR_SUSP;   /* Mark the current process	*/
		prptr->sys_calls_count[3]+=1;
		/*Added by Neehar*/
		asm volatile("CPUID\n\t""RDTSC\n\t""mov %%edx, %0\n\t""mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"(cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
		start = ( ((uint64)cycles_high << 32) | cycles_low );
		end = ( ((uint64)cycles_high1 << 32) | cycles_low1 );
		prptr->sys_calls_total[3] += (end - start);
		/*Added by Neehar*/
		resched();		    /*   suspended and resched.	*/
	}
	prio = prptr->prprio;
	restore(mask);
	return prio;
}
