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

double lambda(double z_hot, double T){

	double	logZ, logT;
	double	tt, tt1, tt2;
	int	idz, i;
	
	idz	=	0;

	if(z_hot < pow(10.0, NIL + (Z_SUN))){
		logZ	=	-10.0;
	}else{
		logZ	=	log10(z_hot);
	}

	logZ	-=	Z_SUN;

	logT	=	log10(T);

//	tt	=	search_row(0, logT);
//	return(tt);

	if(logZ <= NIL){
		idz	=	0;
		tt	=	search_row(idz, logT);
		return(tt);
	}

	if(logZ > 0.5){
		idz	=	7;
		tt	=	search_row(idz, logT);
		return(tt);
	}

	for(i = 0; i <= 6; ++i){
		if(logZ > zTable[i] && logZ <= zTable[i + 1]){
			idz	=	i;
			break;
		}
	}

	tt1	=	search_row(idz, logT);
	tt2	=	search_row(idz + 1, logT);

	tt	=	tt1 + (logZ - zTable[idz]) / (zTable[idz + 1] - zTable[idz]) * (tt2 - tt1);

	return(tt);
}

double search_row(int idz, double logT){
	
	int		idT;
	double	tt, tt1, tt2;

	logT	-=	4.0;
	idT		=	floor(logT / 0.05);

	if(idT <= 0){
		idT	=	0;
		return(z[idz][idT]);
	}

	if(idT >= 90){
		idT	=	90;
		return(z[idz][idT]);
	}

	tt1	=	z[idz][idT];
	tt2	=	z[idz][idT + 1];

	tt	=	tt1 + (logT - idT * 0.05) / 0.05 * (tt2 - tt1);

	return(tt);
}

double cooling_radius(int hid){

	double	Lambda, coolingRadius, T, t_cool;

	T	=	35.9 * g[hid].v_vir * g[hid].v_vir;

	Lambda	=	lambda(g[hid].z_hot, T);
	Lambda	=	pow(10.0, Lambda) * All.UnitCoolingRate;

	t_cool	=	All.t_age;

	coolingRadius	=	Lambda * g[hid].m_hot * t_cool 
					  / (6.0 * M_PI * MU_BAR * All.m_p * All.k * T * g[hid].r_vir);
	
	coolingRadius	=	sqrt(coolingRadius);

	return(coolingRadius);
}

double cooling_rate(int hid){
	
	if(g[hid].type != 0){
		printf("Only central galaxy can cool!\n");
		return(0.0);
	}
	
	if(g[hid].v_vir > MAXVEL){
		g[hid].flag_cool	=	0;
		return(0.0);
	}
	
	double	t_cool, coolingRate;

	t_cool	=	All.t_age;

	if(g[hid].r_vir < g[hid].r_cool){
		coolingRate	=	g[hid].m_hot / t_cool;
	}else{
		coolingRate	=	g[hid].m_hot / g[hid].r_vir * g[hid].r_cool / 2.0 / t_cool;
	}

	return(coolingRate);
}
