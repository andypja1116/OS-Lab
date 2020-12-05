#include<stdio.h>
#include<stdlib.h>
int page_string[150];
int *frame;
int frameNum;
struct node{
	int useful[100];
	int count;
};
struct node * number_of_page;

int findMax(int *array,int arrayNum)
{
	int max=-2;
	int maxpos=0;
	int i;
	for(i=0;i<arrayNum;i++)
	{
		if(max<array[i])
		{
			max=array[i];
			maxpos=i;
		}
	}
	return maxpos;
}

int isExit(int num,int *array,int arrayNum)
{
	int i;
	for(i=0;i<arrayNum;i++)
	{
		if(array[i]==num)
		    return i;
	}
	return -1;
}

void FIFO()
{
	int ptr=0;
	int fault=0;
	int i,j;
	int line=0;
	for(i=0;i<100;i++)
	{
		if(isExit(page_string[i],frame,frameNum)<0)
		{
			fault++;
			frame[ptr]=page_string[i];
			ptr=(ptr+1)%frameNum;
		}
		printf("line=%d\t\t",i);
		for(j=0;j<frameNum;j++)
		{
			printf("%d\t",frame[j]);
		}
		printf("\tNow Referring:%d\n",page_string[i]);
	}
	printf("Total Fault:%d",fault);
}

void LRU()
{
	int line=0;
	int fault=0;
	int *referTimes=(int *)malloc(sizeof(int)*frameNum);
	int i,j;
	for(i=0;i<frameNum;i++)
	{
		referTimes[i]=0;
	}
	for(i=0;i<100;i++)
	{
		int pos=isExit(page_string[i],frame,frameNum);
		if(pos<0)
		{
			fault++;
			pos=findMax(referTimes,frameNum);
			frame[pos]=page_string[i];
		}
		referTimes[pos]=-1;
		for(j=0;j<frameNum;j++)
	    {
		    referTimes[j]++;
	    }
	    printf("line=%d\t\t",i);
	    for(j=0;j<frameNum;j++)
	    {
		    printf("%d\t",frame[j]);
	    }
	    printf("\tNow Referring:%d\n",page_string[i]);
	}
	printf("Total Fault:%d",fault);
}

int Opt_max(int * array,struct node * a,int arrayNum,int current)
{
	int i,max=-2,pos=0,j;
	for(i=0;i<arrayNum;i++)
	{
		if(array[i]==-1)
		    return i;
		j=0;
		for(j=0;j<a[array[i]].count;j++)
		{
            if(a[array[i]].useful[j]>current)
			{
				if(a[array[i]].useful[j]>max)
				{
					max=a[array[i]].useful[j];
				    pos=i;
				}
				break;	
			}
		}
	}
	return pos;
}

void Opt(struct node * array)
{
	int i,j;
	int fault=0;
	for(i=0;i<100;i++)
	{
		int pos=isExit(page_string[i],frame,frameNum);
		if(pos<0)
		{
			fault++;
			pos=Opt_max(frame,array,frameNum,i);
			frame[pos]=page_string[i];
		}
		printf("line=%d\t\t",i);
	    for(j=0;j<frameNum;j++)
	    {
		    printf("%d\t",frame[j]);
	    }
	    printf("\tNow Referring:%d\n",page_string[i]);
	}
	printf("Total Fault:%d",fault);
}

int main()
{
	FILE *fp;
	fp=fopen("data.txt","r");
	int i,j;
	int page;
	int per_page;
	int pageNum;
	printf("Please input the number of instruction per page:");
	scanf("%d",&per_page);
	pageNum=100/per_page;
	number_of_page=(struct node *)malloc(sizeof(struct node)*pageNum);
	for(j=0;j<pageNum;j++)
	    number_of_page[j].count=0;
	for(i=0;i<100;i++)
	{
		fscanf(fp,"%d",&page);
		page=page/per_page;
		page_string[i]=page;
		number_of_page[page].useful[number_of_page[page].count]=i;
		number_of_page[page].count++;
	}
	for(i=0;i<pageNum;i++)
	{
		number_of_page[i].useful[number_of_page[i].count]=100;
		number_of_page[i].count++;
	}
	printf("Please input the total number of the frame:");
	scanf("%d",&frameNum);
	frame=(int *)malloc(sizeof(int)*frameNum);
	for(i=0;i<frameNum;i++)
	{
		frame[i]=-1;
	}
	printf("FIFO\n");
	FIFO();
	for(i=0;i<frameNum;i++)
	{
		frame[i]=-1;
	}
	printf("\n\nLRU\n");
	LRU();
	for(i=0;i<frameNum;i++)
	{
		frame[i]=-1;
	}
	printf("\n\nOpt\n");
	Opt(number_of_page);
}
