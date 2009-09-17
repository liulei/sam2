#ifndef	ALLVARS_H
#define	ALLVARS_H

#include	<stdio.h>

#include	"parameter.h"
#include	"Millennium.h"

#define	MAXLEN_FILENAME		100

#define	GRAVITY			6.673e-11	/*	in mks units	*/
#define	SOLAR_MASS		1.98892e30	/*	in kg	*/
#define	C				2.99792458e8	/*	in m/s	*/
#define	HUBBLE			3.2407789e-18	/*	in h/sec	*/

#define	METER_PER_MPC		3.0856025e22
#define	SEC_PER_GIGAYEAR	3.155e16
#define	SEC_PER_YEAR		3.155e7

typedef struct global_data{

	double	G;
	double	UnitTime_in_s;
	double	UnitMass_in_kg;
	double	UnitVelocity_in_m_per_s;
	double	UnitLength_in_m;
	double	UnitTime_in_Gigayears;
	double	Hubble;

	int		Ntrees;
	int		totalNhalos;
	int		*treeTable;
	
	int		treeNum;
	int		currentSnap;

	char	treeDir[MAXLEN_FILENAME];

	FILE	*fp;
	FILE	*fout;

	int		sn;
	int		it;
	double	dt;
	double	redshift;
	double	snapTime;
	double	snap_z;
	
	FILE	*fdebug;
	FILE	*fsat;
	FILE	*dnum;

	int		groupNum;
	int		galaxyNum;
	int		subNum;
}ALL;

extern	ALL		All;

extern	double	snap_redshift[64];
extern	double	snap_lookback[64];

extern	HALO	*halo;
extern	SNAP	**snap;
extern	int		halo_per_snap[64];

extern	GALAXY	*g;

#endif
