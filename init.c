#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#include	"allvars.h"
#include	"proto.h"

void init(){

	set_units();

	set_snap_time();

	strcpy(All.treeDir, "/home/liulei/SAM/mergingtree/data");

	strcpy(All.outputDir, "result");

	printf("Init done!\n");
}


void set_snap_time(void){	
	
	snap_redshift[0]	=	127.0;
	snap_redshift[1]	=	80.0;
	snap_redshift[2]	=	50.0;
	snap_redshift[3]	=	30.0;

	int	i;
	int	ns;
	for(i = 4; i <= 63; ++i){
		ns	=	63 - i;
		snap_redshift[i]	=	pow(10.0, ns * (ns + 35.) / 4200.) - 1.0;
	}

	for(i = 0; i <= 63; ++i){
		snap_lookback[i]	=	lookback(snap_redshift[i]);
/*		
		printf("%d:\t%f\t%g\n", 
				i, 
				snap_redshift[i], 
				snap_lookback[i] 
//				* All.UnitTime_in_Gigayears
				);
*/
	}
}



void set_units(){

	All.UnitLength_in_m	=	3.085678e22;	/*	1.0 Mpc in m	*/
	All.UnitMass_in_kg	=	1.98892e40;		/*	10^10 solar mass in kg	*/
	All.UnitVelocity_in_m_per_s	=	1000.;	/*	1 km/s in m/s	*/

	All.UnitTime_in_s	=	All.UnitLength_in_m / All.UnitVelocity_in_m_per_s;
	All.UnitTime_in_Gigayears	=	All.UnitTime_in_s / SEC_PER_GIGAYEAR;
	All.G	=	GRAVITY / pow(All.UnitLength_in_m, 3) * All.UnitMass_in_kg * pow(All.UnitTime_in_s, 2);
	All.Hubble	=	HUBBLE * All.UnitTime_in_s;

/*	In the original form Lambda is in units of erg cm^3 s^{-1}, we have to convert it to internal units.*/
	All.UnitCoolingRate	=	1.0e-9 / pow(All.UnitLength_in_m, 3) * All.UnitTime_in_s;
	All.m_p	=	PROTON_MASS / All.UnitMass_in_kg;
	All.k	=	BOLTZMAN_CONST;

	printf("Hubble (internal units) = %g\n", All.Hubble);
	printf("G (internal units) = %g\n", All.G);
	printf("UnitTime_in_s = %g\n", All.UnitTime_in_s);
	printf("UnitTime_in_Gigayears = %g\n", All.UnitTime_in_Gigayears);
}
