#include "seq.h"

int main (int argc,char ** argv)
{
	char aSeq1[MAXSEQLENGTH + 1];	//sequence 1 , +1 to contain '\0'
	char aSeq2[MAXSEQLENGTH + 1];	//sequence 2
	int row=0,column=0;
	MUnit *M;		//the score matrix
	TBChain **headout,*head,*cleaner;

	printf("Enter Sequence 1 and Sequence 2:\n");
	scanf("%s%s",&aSeq1,&aSeq2);
	//printf("%s %s",aSeq1,aSeq2);

	row = Getlength(aSeq2);		//not real rownum,need +1 to add a '0' row
	column = Getlength(aSeq1);	//same,+1
	M = (MUnit*)malloc(sizeof(MUnit)*(row+1)*(column+1));

	if (M == NULL)
	{
		printf("malloc error!\n");
		return -1;
	}

	Mcalc(M,aSeq1,aSeq2); //calculate the matrix
	ShowM(M,aSeq1,aSeq2);
	head = TraceBack(M,aSeq1,aSeq2);		
	if(head == NULL)
	{
		printf("TraceBack Failed!(malloc)\n");
		return -1;
	}

	PrintResultSeq(head);

	// almost forget to do some cleaning work
	while ( (*head).next != NULL)
	{
		cleaner = head;
		head = (*head).next;
		free(cleaner);
		cleaner = NULL;
	} 
	free(head);		//free the last linked list
	head = NULL;
	free(M);		//free the matrix
	M = NULL;
}

void Mcalc(MUnit *M,char *seq1,char *seq2)
{
	int i=0,j=0,row,column;
	int lefttop=0,left=0,top=0,maxn=0,maxp=0;

	row = Getlength(seq2);		//+1 is needed
	column = Getlength(seq1);	//the same
	
	for(i=0;i<(row+1)*(column+1);i++)	//init every unit
	{
		( *(M + i) ).iScore = 0;
		( *(M + i) ).iSi = -1;
		( *(M + i) ).iSj = -1;
	}	

	for(i=0,j=1;j<=column;j++)	//init the first row
	{
		( *(M + j) ).iScore = ( *(M + j-1) ).iScore + SPACESCORE;
		( *(M + j) ).iSi = 0;
		( *(M + j) ).iSj = j-1; 
	}
	for(j=0,i=1;i<=row;i++)		//init the first column
	{
		( *(M + (column+1)*i) ).iScore = ( *(M + (column+1)*(i-1)) ).iScore + SPACESCORE;
		( *(M + (column+1)*i) ).iSi = i-1;
		( *(M + (column+1)*i) ).iSj = 0;
	}
	//now ready to start

	for(i=1;i<=row;i++)
	{
		for(j=1;j<=column;j++)
		{
			lefttop = ( *(M + (column+1)*(i-1)+(j-1)) ).iScore + 
						(seq2[i-1]==seq1[j-1] ? SAMELETTERSCORE : DIFFLETTERSCORE);
			left = ( *(M + (column+1)*i+(j-1)) ).iScore + SPACESCORE;
			top = ( *(M + (column+1)*(i-1)+j) ).iScore + SPACESCORE;
			
			//printf("%d %d %d\n",lefttop,left,top);
			lefttop > left ? (maxn=lefttop,maxp=1) : (maxn=left,maxp=4);
			maxn > top ? 1 : (maxp=2);
			switch (maxp)
			{
				case 1:	( *(M + (column+1)*i+j) ).iScore = lefttop;
						( *(M + (column+1)*i+j) ).iSi = i-1;
						( *(M + (column+1)*i+j) ).iSj = j-1;
						break;
				case 4:	( *(M + (column+1)*i+j) ).iScore = left;
						( *(M + (column+1)*i+j) ).iSi = i;
						( *(M + (column+1)*i+j) ).iSj = j-1;
						break;
				case 2:	( *(M + (column+1)*i+j) ).iScore = top;
						( *(M + (column+1)*i+j) ).iSi = i-1;
						( *(M + (column+1)*i+j) ).iSj = j;
						break;
			}
		}
	}
	//now we finished the matrix
}

TBChain *TraceBack(MUnit *M,char *seq1,char *seq2)
{
	int i,j,prei,prej,row,column;
	TBChain *PreChain=NULL,*Chain;

	row = Getlength(seq2);
	column = Getlength(seq1);
	
	i=row;j=column;
	while(i!=-1 && j!=-1)
	{
		prei = ( *(M + (column+1)*i+j) ).iSi;
		prej = ( *(M + (column+1)*i+j) ).iSj;
		Chain = (TBChain*)malloc(sizeof(TBChain));
		if(Chain == NULL)
		{
			printf("malloc error!\n");
			return NULL;
		}
		if(prei == i || i==0)
			(*Chain).cSeq2 = '-';
		else
			(*Chain).cSeq2 = seq2[i-1];
		if(prej == j || j==0)
			(*Chain).cSeq1 = '-';
		else
			(*Chain).cSeq1 = seq1[j-1];
		i=prei;j=prej;
		(*Chain).next = ((i==row && j==column) ? NULL : PreChain);
		PreChain = Chain;
	}
	return PreChain;	//return the last Chain,also the list head
	
}

void ShowM(MUnit *M,char *seq1,char *seq2)
{
	int i=0,j=0,row,column;
	char fc;		//to create the row/column title

	row = Getlength(seq2);
	column = Getlength(seq1);

	//print title row
	printf("\n\t0\t");
    for(i=0;i<column;i++)
    {
		printf("%c\t",seq1[i]);
    }
	printf("\n");			

	for(i=0;i<=row;i++)
	{
		fc = (i==0)?'0':seq2[i-1];
		printf("%c\t",fc);	//print title column

		for(j=0;j<=column;j++)
		{
			printf("%d\t",( *(M + (column+1)*i+j) ).iScore);	//print unit
		}
		printf("\n");
	}
	printf("\n");	
	 /*
     //print position info
     *//*
     printf("\t0\t");
     for(i=0;i<column;i++)
     {
         printf("%c\t",seq1[i]);
     }
     printf("\n");

     for(i=0;i<=row;i++)
     {
         fc = (i==0)?'0':seq2[i-1];
         printf("%c\t",fc);  //print title column

         for(j=0;j<=column;j++)
         {
             printf("%d,%d\t",( *(M + (column+1)*i+j) ).iSi,( *(M + (column+1)*i+j) ).iSj);    //print unit
         }
         printf("\n\n");
     }
	*/
}

void PrintResultSeq(TBChain* head)
{
	TBChain* seq1=NULL;
	seq1 = head;

	while((*seq1).next != NULL)
	{
		printf("%c\t",(*seq1).cSeq1);
		seq1 = (*seq1).next;
	}
	printf("%c\n",(*seq1).cSeq1);
	
	while((*head).next != NULL)
    {
    	printf("%c\t",(*head).cSeq2);
    	head = (*head).next;
    }
	printf("%c\n",(*head).cSeq2);	
}

int Getlength(char *seq)	//Get sequence length
{
	int i=0;
	while(seq[i] != '\0')
	{
		i++;
	}
	return i;
}
