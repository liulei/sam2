#include	<stdio.h>
#include	<stdlib.h>

#include	"allvars.h"
#include	"proto.h"

void open_file(int fileNum){

	char	fileName[MAXLEN_FILENAME];
	sprintf(fileName, "%s/trees_063.%d", All.treeDir, fileNum);

	if((All.fp = fopen(fileName, "r")) == NULL){
		printf("cannot open file!\n");
		exit(1);
	}

	fread(&All.Ntrees, sizeof(int), 1, fp);
	fread(&All.totalNhalos,sizeof(int), 1, fp);
	printf("Total trees:%d\nTotal halos:%d\n",All.Ntrees,All.totalNhalos);
	All.treeTable   =   (int *)malloc(sizeof(int) * All.Ntrees);
	fread(All.treeTable, sizeof(int), All.Ntrees, All.fp);
}

