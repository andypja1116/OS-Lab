#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	FILE *fp;
	fp=fopen("data.txt","w");
	int address=0,rate,select;
	int i;
	srand((unsigned)time(NULL));
	fprintf(fp,"%d ",address);
	printf("%d",address);
	rate=rand()%10;
	for(i=1;i<100;i++)
	{
		printf("\nrateֵΪ%d:\n",rate);
		if(rate<=6 && rate>=0)//0,1,2,3,4,5,6,7,8,9
		{
			address=address+1;
			fprintf(fp,"%d ",address);
			if(address==99)
			    rate=7;
			else
			    rate=rand()%10;
			printf("%d",address);
		}
		else if(rate==7)
		{
			select=rand()%address;
			address=select;
			fprintf(fp,"%d ",address);
			printf("%d",address);
			rate=rand()%10;
		}
		else
		{
			select=rand()%(99-address);//address=5 10 (6,9) 10-6=4 0,1,2,3 +5+1 =6,7,8,9
			select=select+address+1;
			address=select;
			fprintf(fp,"%d ",address);
			printf("%d",address);
			if(address==99)
			    rate=7;
			else
			    rate=rand()%10;
		}
	}
	return 0;
}
