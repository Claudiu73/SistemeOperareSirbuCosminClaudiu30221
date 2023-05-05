#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "a2_helper.h"

int main()
{
    init();
    info(BEGIN, 1, 0);
    pid_t p2 = fork();
    if(p2 == 0)
    {
        info(BEGIN, 2, 0);
    	info(END, 2, 0);
    	exit(2);
    }
    waitpid(p2, 0, 0);
    pid_t p3 = fork();
    if(p3 == 0)
    {
    	info(BEGIN, 3, 0);
    	pid_t p4 = fork();
    	if(p4 == 0)
    	{
    	     info(BEGIN, 4, 0);
    	     info(END, 4, 0);
    	     exit(4);
    	}
    	waitpid(p4, 0, 0);
    	info(END, 3, 0);
    	exit(3);
    }
    waitpid(p3, 0, 0);
    pid_t p5 = fork();
    if(p5 == 0)
    {
    	info(BEGIN, 5, 0);
    	pid_t p7 = fork();
    	if(p7 == 0)
    	{
    	    info(BEGIN, 7, 0);
    	    info(END, 7, 0);
    	    exit(7);
    	}
    	waitpid(p7, 0, 0);
    	
    	pid_t p9 = fork();
    	if(p9 == 0)
    	{
    	    info(BEGIN, 9, 0);
    	    info(END, 9, 0);
    	    exit(9);
    	}
    	waitpid(p9, 0, 0);
    	info(END, 5, 0);
        exit(5);
    }
    waitpid(p5, 0, 0);
    
    pid_t p6 = fork();
    if(p6 == 0)
    {
    	info(BEGIN, 6, 0);
    	pid_t p8 = fork();
    	if(p8 == 0)
    	{
    	    info(BEGIN, 8, 0);
    	    info(END, 8, 0);
    	    exit(8);
    	}
    	waitpid(p8, 0, 0);
    	info(END, 6, 0);
    	exit(6);
    }
    waitpid(p6, 0, 0);
    info(END, 1, 0);
    exit(1);   
    return 0;
}
