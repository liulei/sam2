#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	"allvars.h"
#include	"proto.h"

void read_tree(int treeNum){

	halo	=	(HALO *)malloc(sizeof(HALO) * All.treeTable[treeNum]);
	fread(halo, sizeof(HALO), All.treeTable[treeNum], All.fp);
}

void construct_tree(int treeNum){
	int	i;
	int	total	=	0;
	int	count_per_snap[64];

	for(i = 0; i < 64; ++i){
		halo_per_snap[i]	=	0;
		count_per_snap[i]	=	0;
	}

	for(i = 0; i < All.treeTable[treeNum]; ++i){
		halo_per_snap[halo[i].snapNum]++;
	}

	snap	=	(SNAP **)malloc(sizeof(SNAP *) * 64);

	for(i = 0; i < 64; ++i){
		snap[i]	=	(SNAP *)malloc(sizeof(SNAP) * halo_per_snap[i]);
		total	+=	halo_per_snap[i];
	}

	printf("Total halos in section %d: %d\n", treeNum, total);

	int	snapNum, count;
	for(i = 0; i < All.treeTable[treeNum]; ++i){
		snapNum	=	halo[i].snapNum;
		count	=	count_per_snap[snapNum];
		snap[snapNum][count].hid	=	i;
		count_per_snap[snapNum]++;
	}
}
