/*  main.c  - main */

#include <xinu.h>
unsigned long	*sp = 0, *bp = 0, *fp = 0;
int count = 0;


int function_two_para(int a, int b){
	int sum;
	sum=a+b;
	asm("movl %%esp, %0\n" :"=r"(sp));
	asm("movl %%ebp, %0\n" :"=r"(fp));
	kprintf("ebp inside function is %X", fp);
	kprintf("-------------------\n");
	count = 0;
		while(sp<=bp)
		{
			kprintf("Inside function_two_para stack is value address:%X is %X\n", sp, *sp);
			sp++;
			count++;
			if(count == 12)
			{
				break;
			}
		}

//CASE1:AFTER
	//asm("movl %%esp, %0\n" :"=r"(sp));
	//asm("movl %%ebp, %0\n" :"=r"(fp));
	//kprintf("Inside function_two_para sp %X fp %X\n", sp, fp);
	printf("\nThe sum of the two parameters %d and %d is: %d\n", a, b, sum);
	return OK;
}

int function_three_para(int a, int b, int c){
	int sum;
	sum=a+b+c;
//CASE2:AFTER
	asm("movl %%esp, %0\n" :"=r"(sp));
	kprintf("-------------------\n", sp, *sp);
	
	count = 0;
	while(sp<=bp)
	{
		kprintf("Inside function_three_para stack is value address:%X is %X\n", sp, *sp);
		sp++;
		count++;
		if(count == 12)
		{
			break;
		}
	}

	printf("\nThe sum of the three parameters %d, %d and %d is: %d\n", a, b, c, sum);
	return OK;
}

process	main(void)
{
	asm("movl %%ebp, %0\n" :"=r"(bp));
	asm("movl %%esp, %0\n" :"=r"(sp));
		while(sp<=bp)
		{
			kprintf("Inside process main stack is value address:%X is %X\n", sp, *sp);
			sp++;
			count++;
			if(count == 12)
			{
				break;
			}
		}

	//test function with two parameters
//CASE1:BEFORE
	function_two_para(1, 2);
	asm("movl %%esp, %0\n" :"=r"(sp));
	count = 0;
		kprintf("-------------------\n");

	while(sp<=bp)
	{
		kprintf("After returning function_two_para stack is value address:%X is %X\n", sp, *sp);
		sp++;
		count++;
		if(count == 12)
		{
			break;
		}
	}
	//test function with three parameters
//CASE2:BEFORE
	
	function_three_para(1, 2, 3);
	asm("movl %%esp, %0\n" :"=r"(sp));
	count = 0;
		kprintf("-------------------\n");

	while(sp<=bp)
	{
		kprintf("After returning function_three_para stack is value address:%X is %X\n", sp, *sp);
		sp++;
		count++;
		if(count == 12)
		{
			break;
		}
	}
	

	/* Run the Xinu shell */
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		pr_status_syscall_summary();
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
