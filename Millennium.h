/*	halo structure for reading the data provided by Liyun	*/
#ifndef	MILLENNIUM_H
#define	MILLENNIUM_H

typedef struct tagHaloStruct{
	int	descendant;
	int	firstProgenitor;
	int	nextProgenitor;
	int	firstHaloInFOFgroup;
	int	nextHaloInFOFgroup;
	int	len;
	float	m_Mean200;
	float	m200;
	float	m_TopHat;
	float	pos[3];
	float	vel[3];
	float	velDisp;
	float	vmax;
	float	spin[3];
	long long	mostBoundID;
	int	snapNum;
	int	fileNr;
	int	subhaloIndex;
	float	subhalfMass;
}HALO;

typedef struct tagGalaxy{
/*  basic parameter of galaxies come from halo data:	*/
	int		type;
	int		snapNum;
	float	redshift;
	int		isLive;
	int	origin;
	int	firstProg;
	int	nextProg;
	int	firstInFOF;
	int	nextInFOF;
	float	m_vir;
	float	r_vir;
	float	v_vir;
	float	t_dyn;
	float	v_c;
	float	sfr;
	float	pos[3];
	float	vel[3];

/*	for central galaxy:	*/
	float	coolingRate;
	float	r_cool;
	float	flag_cool;

/*	for all galaxy:	*/
	int	currentMain;
	int	nextSat;

/*	to better trace the merge history of galaxy, here the last merge store
 * 	the origin id of the satellite:
 */
	int	lastMerge;

/*	for satellite galaxy:	*/
	float	t_merge;
	int	center;

/*	parameters need by SAM:	*/
	float	m_eject_step;
	float	m_eject_z_step;
	float	m_hot;
	float	m_hot_z;
	float	m_cold;
	float	m_cold_z;
	float	m_stellar;
	float	m_stellar_z;
	float	m_bulge;
	float	m_bulge_z;
	float	z_hot;
	float	z_cold;

/*	this flag is only useful for output, when not visited, set it to 0,
 *	otherwise set to 1
 */
	int		flag_v;
}GALAXY;
	
typedef struct tagSnap{
	int		hid;
//	int		flag;
//	Galaxy*	galaxy;
}SNAP;

typedef struct tagGalaxyOut{

/*	id of galaxy in the model
 */
	int	hid;

/*	id of the FOF group
 */
	int	haloID;

/*	galaxy id in the FOF group, id of central galaxy will be 0
 */
	int	subID;

/*	mass of the FOF group
 */
	float	centralMvir;

/*	stellar mass of the galaxy
 */
	float	stellarMass;
}GOUT;

#endif
