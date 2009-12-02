#include	<stdio.h>
#include	<stdlib.h>

#include	"allvars.h"
#include	"proto.h"

static	int	groupNum;
static	int	galaxyNum;
static	int	subNum;

static	float	centralMvir;

static	GOUT	gout;

void open_file(int fileNum){

	char	fileName[MAXLEN_FILENAME];
	sprintf(fileName, "%s/trees_063.%d", All.treeDir, fileNum);

	if((All.fp = fopen(fileName, "r")) == NULL){
		printf("cannot open file!\n");
		exit(1);
	}

	fread(&All.Ntrees, sizeof(int), 1, All.fp);
	fread(&All.totalNhalos,sizeof(int), 1, All.fp);
	printf("Total sections:%d\nTotal halos:%d\n",All.Ntrees,All.totalNhalos);
	All.treeTable   =   (int *)malloc(sizeof(int) * All.Ntrees);
	fread(All.treeTable, sizeof(int), All.Ntrees, All.fp);
}

void begin_output(int fileNum){

	char	fileName[MAXLEN_FILENAME];
	sprintf(fileName, "%s/local.%d", All.outputDir, fileNum);

	if((All.fout = fopen(fileName, "w")) == NULL){
		printf("Cannot open output file %d!\n", fileNum);
		exit(1);
	}

	groupNum	=	0;
	galaxyNum	=	0;
	subNum		=	0;

	printf("Writing total number...");
	fwrite(&galaxyNum, sizeof(int), 1, All.fout);
	fwrite(&groupNum, sizeof(int), 1, All.fout);
	printf("done!\n");
}

void end_output(){
	fseek(All.fout, 0, SEEK_SET);
	fwrite(&galaxyNum, sizeof(int), 1, All.fout);
	fwrite(&groupNum, sizeof(int), 1, All.fout);
	fclose(All.fp);
	fclose(All.fout);
	
	printf("Total galaxy: %d\n", galaxyNum);
	printf("Total group: %d\n", groupNum);
}

void output_tree(int treeNum){

	int	i, hid, pid;
	for(i = 0; i < halo_per_snap[OUTSNAP]; ++i){

		hid	=	snap[OUTSNAP][i].hid;

		if(g[hid].flag_v == 0){

			pid	=	g[hid].firstInFOF;
			centralMvir	=	g[pid].m_vir;
			subNum	=	0;
			while(pid != -1){
				output_galaxy(pid);
				g[pid].flag_v	=	1;
				pid	=	g[pid].nextInFOF;
			}
			groupNum++;
		}
	}

	if(halo != NULL){
		free(halo);
		halo	=	NULL;
	}
	if(g != NULL){
		free(g);
		g	=	NULL;
	}
}

void output_galaxy(int hid){
	
	int	sid;
	
	write_single_galaxy(hid);
	sid	=	g[hid].nextSat;
	
	while(sid != -1){
		write_single_galaxy(sid);
		sid	=	g[sid].nextSat;
	}
}

void write_single_galaxy(int hid){
	
	gout.hid	=	hid;
	gout.haloID	=	groupNum;
	gout.subID	=	subNum;
	gout.centralMvir	=	centralMvir;
	gout.stellarMass	=	g[hid].m_stellar;

	fwrite(&gout, sizeof(GOUT), 1, All.fout);

	galaxyNum++;
	subNum++;
}
