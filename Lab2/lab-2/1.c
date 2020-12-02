#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	int num;
	scanf("%d",&num);
	pid_t pid;
	int status;
	pid=fork();
	if(pid==0)
	{
	        printf("%d ",num);
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
			printf("%d ",num);
		}
		printf("\n");
		exit(0);
	}
	else
	{
		wait(&status);
		printf("Child Complete!");
		exit(0);
	}
}
