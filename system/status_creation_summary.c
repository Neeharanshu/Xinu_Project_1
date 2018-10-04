#include<xinu.h>

void pr_status_syscall_summary()
{

struct procent *prptr;
char* sys_calls_arr[10] = {"Create", "Yield", "Sleep", "Suspend", "Exit", "Receive", "Recvtime", "Ready", "Unsleep", "Wait"};
int j = 0;
prptr = &proctab[currpid];
int i = 0;
for(i = 0; i < NPROC; i++)
{
	prptr = &proctab[i];
	kprintf("PID    syscall     count      average     no. of cycles\n");
	kprintf("---    -------     -----      -------     --------------\n");
	for (j = 0; j < 10; j++)
	{
	if(prptr->sys_calls_count[j] == 0)
	{
		
	}
	else{
		kprintf("%d     %s     %d     %d     %d\n", i,sys_calls_arr[j],prptr->sys_calls_count[j], (prptr->sys_calls_total[j]/prptr->sys_calls_count[j]), prptr->sys_calls_total[j]);
	}
	}
	kprintf("\n------------------------------------------------\n");
}
}
