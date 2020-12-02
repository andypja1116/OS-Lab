#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include<sys/wait.h>
 
struct shareMemory{
    int col[200]; 
};
 
int main(int argc, const char *argv[])
{
    key_t  key_n ;
    int    shm_id;
    pid_t  pid;
    int status;
    void *shm = NULL;  //共享内存的首地址
    struct shareMemory *shared;
    int num;
    scanf("%d",&num);
 
    if((key_n = ftok("/",'s')<0))  //生成共享内存的key值
    {
        perror("Fail to ftok");
        exit(EXIT_SUCCESS);
    }
 
    if((shm_id = shmget(key_n,1024,0666|IPC_CREAT))==-1)  //创建共享内存
    {
        perror("Fail to shmget");
        exit(EXIT_SUCCESS);
    }
 
    if((pid = fork())==-1)
    {
        perror("Fail to fork");
        exit(EXIT_SUCCESS);
    }
 
    if(pid==0)                                         //子进程，用来读
    {
    	int i=1;
        shm = shmat(shm_id,0,0);                     //将共享内存连接到当前进程的地址空间
        if(shm==(void *)-1)
        {
            perror("Fail to shmat");
            exit(EXIT_SUCCESS);
        }
 
        shared = (struct shareMemory *)shm;        //设置共享内存
        shared->col[1]=num;
        while(num!=1)
        {
        	if(num%2==0)
        	{
        		num=num/2;
		}
		else
		{
			num=num*3+1;
		}
		i++;
		shared->col[i]=num;
	}
	shared->col[0]=i;
        if(shmdt(shm)==-1)   //把共享内存从当前进程分离
        {
            perror("Fail to shmdt");
            exit(EXIT_SUCCESS);
        }
        exit(0);
    }
 
    if(pid!=0)
    {
    	wait(&status);
        shm = shmat(shm_id,0,0);
        if(shm==(void *)-1)
        {
            perror("Fail to shmat");
            exit(EXIT_SUCCESS);
        }
        shared = (struct shareMemory *)shm;
        int j=1;
        for(j=1;j<=shared->col[0];j++)
        {
        	printf("%d ",shared->col[j]);
        }
		
 
 
        if(shmdt(shm)==-1)
        {
            perror("Fail to shmdt");
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_SUCCESS);
    }
 
    return 0;
}
