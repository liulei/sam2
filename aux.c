#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	"allvars.h"
#include	"proto.h"

double lookback(double z){

	int	i, N;
	double	dz, tot, zz, aa;

	dz	=	0.001;
	tot	=	0.0;
	N	=	z / dz;
	for(i = 0; i < N; ++i){
		zz	=	(i + 0.5) * dz;
		aa	=	sqrt(1.0 - Omega_m + Omega_m * pow(1.0 + zz, 3));
		tot	+=	1. / ((1.0 + zz) * aa);
	}

	tot	=	tot * dz / All.Hubble;

	return tot;
}
