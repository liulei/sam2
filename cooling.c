#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	"allvars.h"
#include	"proto.h"

static float zTable[8];
static float z[8][91];

void init_cooling(){
	
	char fileName[50];
	FILE *fp;
	int	i;

	printf("Initiating cooling curves...\n");

	zTable[0]	=	NIL;
	zTable[1]	=	-3.0;
	zTable[2]	=	-2.0;
	zTable[3]	=	-1.5;
	zTable[4]	=	-1.0;
	zTable[5]	=	-0.5;
	zTable[6]	=	0.0;
	zTable[7]	=	0.5;

	for(i = 0; i <= 7; ++i){
		
		sprintf(fileName, "CIE/lambda%d.dat", i);
		if((fp = fopen(fileName, "r")) == NULL){
			printf("Error while open %s!\n", fileName);
			exit(1);
		}
		
		fread(&z[i][0], sizeof(float), 91, fp);

		fclose(fp);

		printf("z = %f\n", zTable[i]);

	}

}


double cooling_rate(int hid){

	return(0);

}
