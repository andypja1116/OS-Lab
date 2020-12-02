#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>

int main()
{
	int ret1,ret2;
	int fd1[2];
	int fd2[2];
	int status;
	pid_t pid;
	ret1=pipe(fd1);
	ret2=pipe(fd2);
	pid=fork();
	if(pid>0)
	{
		char msg[50]="My name is Pja,my English name is Andy";
		char result[50];
		close(fd1[0]);
		write(fd1[1],msg,strlen(msg)+1);
		wait(&status);
		close(fd2[1]);
		read(fd2[0],result,50);
		printf("The orign string is: %s\n",msg);
		printf("The string after change is:%s",result);
		close(fd1[1]);
		close(fd2[2]);
	}
	else
	{
		char read_msg[50];
		close(fd1[1]);
		read(fd1[0],read_msg,50);
		close(fd2[0]);
		int i;
		for(i=0;i<strlen(read_msg);i++)
		{
			if(read_msg[i]>='a'&&read_msg[i]<='z')
			{
				read_msg[i]=read_msg[i]-32;
			}
			else if(read_msg[i]>='A'&&read_msg[i]<='Z')
			{
				read_msg[i]=read_msg[i]+32;
			}
		}
		write(fd2[1],read_msg,strlen(read_msg)+1);
		close(fd2[1]);
		close(fd1[0]);
		exit(0);
	}
}
