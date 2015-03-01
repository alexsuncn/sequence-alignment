#include <stdio.h>
#include <malloc.h>

#define MAXSEQLENGTH 60
#define SAMELETTERSCORE 1
#define DIFFLETTERSCORE -1
#define SPACESCORE -2

typedef struct MatrixUnit{	//Single Unit in the score matrix
int iSi;
int iSj;
int iScore;
}MUnit;

typedef struct TraceBackChain{	//Linked list to traceback and save result
	char cSeq1;
	char cSeq2;
	struct TraceBackChain *next;
}TBChain;

void Mcalc(MUnit* ,char* ,char* );
TBChain *TraceBack(MUnit* ,char* ,char* );
void ShowM(MUnit* ,char* ,char* );
void PrintResultSeq(TBChain* );
int Getlength(char* );

