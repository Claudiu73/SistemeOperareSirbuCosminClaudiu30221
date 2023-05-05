#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "a2_helper.h"

typedef struct 
{
    int process_no;
    int thread_no;
}InfoStruct;

sem_t *s1;
sem_t *s2;

void* thread_fn(void* arg)
{
    InfoStruct* a = (InfoStruct*) arg;
    if(a->process_no == 9 && a->thread_no == 3)
    {
         sem_wait(s1);
    }
    info(BEGIN, a->process_no, a->thread_no);
    
    if(a->process_no == 9 && a->thread_no == 5)
    {
    	sem_post(s1); 
        sem_wait(s2);
    }
    info(END, a->process_no, a->thread_no);
    if(a->process_no == 9 && a->thread_no == 3)
    {
    	sem_post(s2);
    }
    return NULL;
}
int main()
{
    init();
    sem_unlink("s1");
    sem_unlink("s2");
    s1=sem_open("s1", O_CREAT, 0644, 0);
    s2=sem_open("s2", O_CREAT, 0644, 0);
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
    	    
    	    pthread_t tid[5];
    	    InfoStruct v[5];
    	    for(int i = 0; i < 5; i++)
    	    {
    	    	v[i].process_no = 9;
		v[i].thread_no = i + 1;
    	        pthread_create(&tid[i], NULL, thread_fn, &v[i]);
    	    }
    	    
    	    for(int i = 0; i < 5; i++)
    	    {
    	        pthread_join(tid[i], NULL);
    	    }
    	    
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
