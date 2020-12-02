#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
int read_count=0;
sem_t mutex,RP_Write;
int que[1000];
int pointer=0;
int header=0;

struct thread_arg{
int number;
double create;
int last;
};

void mysleep(int second)
{
    clock_t start;
    start=clock();
    while(((clock()-start)/CLOCKS_PER_SEC)<second);
}

void RP_ReaderThread(void * arg)
{
    sem_wait(&mutex);
    read_count++;
    if(read_count==1)
        sem_wait(&RP_Write);
    sem_post(&mutex);
    int add;
    struct thread_arg *threadarg;
    threadarg=(struct thread_arg *)arg;
    clock_t enter;
    clock_t leave;
    enter=clock();
    mysleep(threadarg->last);
    printf("\nRead Thread %d is created:\n",threadarg->number);
    printf("Create Time=%f\n",threadarg->create);
    printf("Enter critical section=%f\n",(double)enter/CLOCKS_PER_SEC);
    add=rand()%100;
    if(pointer==0)
        printf("The queue is empty\n");
    else
    {
        add=que[header];
        printf("Pop %d from queue\n",add);
        header++;
    }
    leave=clock();
    printf("Leave critical section=%f\n",(double)leave/CLOCKS_PER_SEC);
    sem_wait(&mutex);
    read_count--;
    if(read_count==0)
        sem_post(&RP_Write);
    sem_post(&mutex);
}

void RP_WriterThread(void * arg)
{

    sem_wait(&RP_Write);
    int add;
    struct thread_arg *threadarg;
    threadarg=(struct thread_arg *)arg;
    clock_t enter;
    clock_t leave;
    enter=clock();
    mysleep(threadarg->last);
    printf("\nWrite Thread %d is created:\n",threadarg->number);
    printf("Create Time=%f\n",threadarg->create);
    printf("Enter critical section=%f\n",(double)enter/CLOCKS_PER_SEC);
    add=rand()%100;
    que[pointer]=add;
    pointer++;
    printf("Add %d to queue\n",add);
    leave=clock();
    printf("Leave critical section=%f\n",(double)leave/CLOCKS_PER_SEC);
    sem_post(&RP_Write);
}

int main()
{
    pthread_t thread[100];
    char name[20],mode,useless;
    int row;
    int i;
    int last;
    clock_t create;
    struct thread_arg arg[100];
    clock_t time;
    scanf("%s %d",name,&row);
    FILE *fp;
    fp=fopen(name,"r");
    sem_init(&mutex,0,1);
    sem_init(&RP_Write,0,1);
    for(i=0;i<row;i++)
    {
        arg[i].number=i;
        fscanf(fp,"%c %d%c",&mode,&last,&useless);
        arg[i].last=last;
        if(mode=='R')
        {
            create=clock();
            arg[i].create=(double)create/CLOCKS_PER_SEC;
            pthread_create(&thread[i],NULL,(void *)&RP_ReaderThread,(void *)&arg[i]);
        }
        else if(mode=='W')
        {
            create=clock();
            arg[i].create=(double)create/CLOCKS_PER_SEC;
            pthread_create(&thread[i],NULL,(void *)&RP_WriterThread,(void *)&arg[i]);
        }
    }
    for(i=0;i<row;i++)
    {
        pthread_join(thread[i],NULL);
    }
    return 0;
}
