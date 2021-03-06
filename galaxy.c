#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	"allvars.h"
#include	"proto.h"

static int	it;

static double	dt;
static double	dz;

//	static double	t_age;
//	static double	redshift;

void run(int treeNum){
	
	int	i, hid, snapNum;

	g	=	(GALAXY *)malloc(sizeof(GALAXY) * All.treeTable[treeNum]);

	printf("section %d: \n", treeNum);

//	printf("create galaxy...\n");
	All.t_age	=	AGE - snap_lookback[STARTSNAP];
	for(i = 0; i < halo_per_snap[STARTSNAP]; ++i){
		hid	=	snap[STARTSNAP][i].hid;
		create_galaxy(hid);
	}

	for(i = 0; i < halo_per_snap[STARTSNAP]; ++i){
		hid	=	snap[STARTSNAP][i].hid;
		if(g[hid].type == 0){
			update_center_hot(hid);
			g[hid].coolingRate	=	cooling_rate(hid);
		}
	}

//	printf("evolve galaxy...\n");
	for(snapNum = STARTSNAP + 1; snapNum <= ENDSNAP; ++snapNum){
		
		evolve_galaxy(snapNum);

//		printf("total halos in snap %d: %d\n", snapNum, halo_per_snap[snapNum]);
		for(i = 0; i < halo_per_snap[snapNum]; ++i){
			hid	=	snap[snapNum][i].hid;
			construct_galaxy(hid, snapNum);
		}

		All.t_age	=	AGE - snap_lookback[snapNum];

		for(i = 0; i < halo_per_snap[snapNum]; ++i){
			hid	=	snap[snapNum][i].hid;
			if(g[hid].type == 0){
				update_center_hot(hid);
				g[hid].coolingRate	=	cooling_rate(hid);
			}
		}
	}
}

void update_center_hot(int hid){

	int		pid, sid;
	double	totalBaryon, totalHotZ, totalHalo;
	
	pid		=	hid;

	totalBaryon	=	0.0;
	totalHotZ	=	0.0;
	totalHalo	=	0.0;

	totalHotZ	+=	g[pid].m_eject_z_step;
	g[pid].m_eject_z_step	=	0.0;

	while(pid != -1){
		
		totalHotZ	+=	g[pid].m_hot_z;
		g[pid].m_hot_z	=	0;

		totalBaryon	+=	g[pid].m_cold + g[pid].m_stellar;

		sid	=	g[pid].nextSat;
		while(sid != -1){
			totalBaryon	+=	g[sid].m_cold + g[sid].m_stellar;

			totalHotZ	+=	g[sid].m_hot_z;
			g[sid].m_hot_z	=	0.0;

			sid	=	g[sid].nextSat;
		}

		pid	=	g[pid].nextInFOF;
	}

	g[hid].m_hot	=	g[hid].m_vir * f_b - totalBaryon;
	g[hid].m_hot_z	=	totalHotZ;
	
	if(g[hid].m_hot < LOWINF){
		g[hid].m_hot	=	0.0;
		g[hid].m_hot_z	=	0.0;
		g[hid].z_hot	=	0.0;
	}else{
		g[hid].z_hot	=	g[hid].m_hot_z / g[hid].m_hot;
	}
	
	if(g[hid].z_hot >= 1.0){
		printf("Too many metal gas!\n");
		g[hid].z_hot	=	1.0;
		g[hid].m_hot_z	=	g[hid].m_hot;
	}
}

void create_galaxy(int hid){
	
	int	i, pid;

	g[hid].origin	=	hid;
	g[hid].firstProg	=	halo[hid].firstProgenitor;
	g[hid].nextProg		=	halo[hid].nextProgenitor;
	g[hid].firstInFOF	=	halo[hid].firstHaloInFOFgroup;
	g[hid].nextInFOF	=	halo[hid].nextHaloInFOFgroup;

	if(hid == g[hid].firstInFOF){
		g[hid].type =	0;
	}else{
		g[hid].type	=	1;
	}

	g[hid].isLive	=	1;

	g[hid].snapNum	=	halo[hid].snapNum;
	g[hid].redshift	=	snap_redshift[g[hid].snapNum];

//	g[hid].m_vir	=	halo[hid].m_Mean200;
	g[hid].m_vir	=	halo[hid].m200;
//	g[hid].m_vir	=	halo[hid].m_TopHat;

	if(g[hid].type == 0 && g[hid].m_vir < LOWINF){
//		printf("%d: m200: %f\tlen: %d\n", hid, halo[hid].m200, halo[hid].len);
		g[hid].m_vir	=	0.086 * halo[hid].len;
	}

	if(g[hid].type != 0){
		g[hid].m_vir	=	0.086 * halo[hid].len;
	}

	g[hid].r_vir	=	r_vir(g[hid].m_vir, g[hid].redshift);
	if(g[hid].type == 0){
		g[hid].v_vir	=	v_vir(g[hid].m_vir, g[hid].redshift);
		g[hid].t_dyn	=	t_dyn(g[hid].m_vir, g[hid].redshift);
	}else{
		if(g[hid].firstProg == -1){
			g[hid].v_vir	=	v_vir(g[hid].m_vir, g[hid].redshift);
			g[hid].t_dyn	=	t_dyn(g[hid].m_vir, g[hid].redshift);
		}else{
			pid	=	g[hid].firstProg;
			g[hid].v_vir	=	g[pid].v_vir;
			g[hid].t_dyn	=	g[pid].t_dyn;
		}
	}

	g[hid].v_c	=	halo[hid].velDisp / 1.414;

	g[hid].sfr	=	0.0;

	for(i = 0; i < 3; ++i){
		g[hid].pos[i]	=	halo[hid].pos[i];
		g[hid].vel[i]	=	halo[hid].vel[i];
	}

	g[hid].coolingRate	=	0.0;
	g[hid].r_cool		=	0.0;
	g[hid].flag_cool	=	1;

	g[hid].currentMain	=	g[hid].firstInFOF;
	
	g[hid].nextSat		=	-1;
	g[hid].lastMerge	=	-1;
	
	g[hid].t_merge	=	-1000.0;
	g[hid].center	=	-1;

	g[hid].m_eject_step		=	0.0;
	g[hid].m_eject_z_step	=	0.0;
	g[hid].m_hot			=	g[hid].m_vir * f_b;
	g[hid].m_hot_z			=	0.0;
	g[hid].m_cold			=	0.0;
	g[hid].m_cold_z			=	0.0;
	g[hid].m_stellar		=	0.0;
	g[hid].m_stellar_z		=	0.0;
	g[hid].m_bulge			=	0.0;
	g[hid].m_bulge_z		=	0.0;
	g[hid].z_hot			=	0.0;
	g[hid].z_cold			=	0.0;

	g[hid].flag_v	=	0;
}

void evolve_galaxy(int snapNum){

	double	snapTime, snapZ;
	int		i, hid, sid;

	snapTime	=	snap_lookback[snapNum - 1] - snap_lookback[snapNum];
	snapZ		=	snap_redshift[snapNum - 1] - snap_redshift[snapNum];
	dt	=	snapTime / NSTEP;
	dz	=	snapZ / NSTEP;

	for(it = 1; it <= NSTEP; ++it){
		All.t_age	=	it * dt + AGE - snap_lookback[snapNum - 1];
		All.redshift=	snap_redshift[snapNum - 1] - (it - 1) * dz;
		
		for(i = 0; i < halo_per_snap[snapNum - 1]; ++i){
			hid	=	snap[snapNum - 1][i].hid;
			evolve_single_galaxy(hid);
		}
	}

	for(i = 0; i < halo_per_snap[snapNum - 1]; ++i){
		hid	=	snap[snapNum - 1][i].hid;
		g[hid].snapNum++;
		sid	=	g[hid].nextSat;
		while(sid != -1){
			g[sid].snapNum++;
			sid	=	g[sid].nextSat;
		}
	}
}

int construct_galaxy(int hid, int snapNum){

	create_galaxy(hid);

	if(g[hid].firstProg == -1){
		return(0);
	}

	int pid, psid;
	int	center, currentMain;

	center	=	hid;
	currentMain	=	g[hid].firstInFOF;

	pid	=	g[hid].firstProg;
	while(g[pid].snapNum < snapNum){
		jump_galaxy(pid);
	}

	g[hid].origin	=	g[pid].origin;
	g[hid].lastMerge	=	g[pid].lastMerge;

	/* update halo/center galaxy and their satellite,
	 * return last satellite of this galaxy
	 */
	psid	=	update_halo_galaxy(hid);

	pid		=	g[pid].nextProg;

	g[psid].nextSat	=	pid;

	while(pid != -1){
		
		while(g[pid].snapNum < snapNum){
			jump_galaxy(pid);
		}
	
		psid	=	halo_to_sat(pid, center, currentMain);

		pid		=	g[pid].nextProg;

		g[psid].nextSat	=	pid;
	}

	return(0);
}

void evolve_single_galaxy(int hid){
	
	int	psid, sid;
	
	integrate(hid);

	psid	=	hid;
	sid		=	g[hid].nextSat;

	while(sid != -1){
		integrate(sid);
		g[sid].t_merge	-=	dt;
		if(g[sid].t_merge < 0.0){
			g[psid].nextSat	=	g[sid].nextSat;
			merge(sid);
			g[sid].isLive	=	0;
		}else{
			psid	=	sid;
		}

		sid	=	g[sid].nextSat;
	}
}


int jump_galaxy(int pid){

	if(g[pid].type > 1){
		printf("wrong type for jump!\n");
		return(1);
	}

	int		snapNum, sid;
	double	snapTime, snapZ;

	snapNum		=	g[pid].snapNum;
	snapTime	=	snap_lookback[snapNum] - snap_lookback[snapNum + 1];
	snapZ		=	snap_redshift[snapNum] - snap_redshift[snapNum + 1];

	dt	=	snapTime / NSTEP;
	dz	=	snapZ / NSTEP;

	for(it = 0; it < NSTEP; ++it){

		All.t_age	=	it * dt + AGE - snap_lookback[snapNum];
		All.redshift=	snap_redshift[snapNum] - it * dz;
		
		evolve_single_galaxy(pid);
	}
	
	g[pid].snapNum++;
	
	sid	=	g[pid].nextSat;
	while(sid != -1){
		
		g[sid].snapNum++;
		if(g[sid].snapNum != g[pid].snapNum){
			printf("error for satellite jump!\n");
		}
		
		sid	=	g[sid].nextSat;
	}
	
	return(0);
}

int halo_to_sat(int pid, int center, int currentMain){

	int	psid, pcid;
	double	r_sat;

	g[pid].currentMain	=	currentMain;

	g[pid].type	=	2;

	g[pid].center	=	center;

	pcid	=	g[center].firstProg;

	if(pcid == -1){
		r_sat	=	g[center].r_vir;
	}else{
    	r_sat   =   (g[pid].pos[0]-g[pcid].pos[0])*(g[pid].pos[0]-g[pcid].pos[0])
					+ (g[pid].pos[1]-g[pcid].pos[1])*(g[pid].pos[1]-g[pcid].pos[1])
					+ (g[pid].pos[2]-g[pcid].pos[2])*(g[pid].pos[2]-g[pcid].pos[2]);	

//	???????????????????????????????????????????????????????????
		r_sat   =   sqrt(r_sat)/(1.+g[pcid].redshift);
//	???????????????????????????????????????????????????????????
	}

	g[pid].t_merge  =   merge_time(center, pid, r_sat);
	
	psid	=	update_all_sat(pid, center, currentMain, 1, r_sat);

	return(psid);
}

int update_halo_galaxy(int hid){

	int	pid, psid;

	pid	=	g[hid].firstProg;
	
	g[hid].nextSat	=	g[pid].nextSat;

	g[hid].flag_cool	=	g[pid].flag_cool;

	g[hid].m_eject_step		=	0.0;
	g[hid].m_eject_z_step	=	g[pid].m_eject_z_step;
	g[hid].m_hot		=	0.0;
	g[hid].m_hot_z		=	g[pid].m_hot_z;
	g[hid].m_cold		=	g[pid].m_cold;
	g[hid].m_cold_z		=	g[pid].m_cold_z;
	g[hid].m_stellar	=	g[pid].m_stellar;
	g[hid].m_stellar_z	=	g[pid].m_stellar_z;
	g[hid].m_bulge		=	g[pid].m_bulge;
	g[hid].m_bulge_z	=	g[pid].m_bulge_z;
	g[hid].z_hot		=	0.0;
	g[hid].z_cold		=	g[pid].z_cold;

	int	center, currentMain;
	center	=	hid;
	currentMain	=	g[hid].firstInFOF;

	psid	=	update_all_sat(hid, center, currentMain, 0, 0);

	return(psid);
}

int update_all_sat(int pid, int center, int currentMain, int isUpdate, double r_sat){

	int	psid, sid;
	
	psid	=	pid;
	sid		=	g[pid].nextSat;
	
	while(sid != -1){
		update_single_sat(sid, center, currentMain, isUpdate, r_sat);
		psid	=	sid;
		sid		=	g[sid].nextSat;
	}

	return(psid);
}

void update_single_sat(int pid, int center, int currentMain, int isUpdate, double r_sat){
	
	g[pid].currentMain	=	currentMain;

	g[pid].center	=	center;

	if(isUpdate == 1){
		g[pid].t_merge	=	merge_time(center, pid, r_sat);
	}
}

int merge(int sid){
	
	double	m_small, m_big;
	int		cid;

	cid	=	g[sid].center;
	if(g[cid].snapNum != g[sid].snapNum){
		printf("Error merge time!\n");
		return(1);
	}

	m_small	=	g[sid].m_stellar;
	m_big	=	g[cid].m_stellar;

	float	ratio;

	if(m_big < LOWINF){
//		printf("merge: galaxy %d has no star!\n", cid);
		ratio	=	1.0;
	}else{
//		printf("merge:\n");
		ratio	=	m_small / m_big;
	}

	if(ratio < MC){

/*	minor merge:
 */
		g[cid].m_cold		+=	g[sid].m_cold;
		g[cid].m_cold_z		+=	g[sid].m_cold_z;
		g[cid].m_bulge		+=	g[sid].m_stellar;
		g[cid].m_bulge_z	+=	g[sid].m_stellar_z;

		g[cid].m_stellar	+=	g[sid].m_stellar;
		g[cid].m_stellar_z	+=	g[sid].m_stellar_z;
	
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * don't forget set g[cid].z_cold!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

	}else{
/*	major merge:
 */
		g[cid].m_bulge		=	g[cid].m_stellar + g[cid].m_cold
						  	  + g[sid].m_stellar + g[sid].m_cold;
		g[cid].m_bulge_z	=	g[cid].m_stellar_z + g[cid].m_cold_z 
							  + g[sid].m_stellar_z + g[sid].m_cold_z;

		g[cid].m_stellar	=	g[cid].m_bulge;
		g[cid].m_stellar_z	=	g[cid].m_bulge_z;

		g[cid].m_cold	=	0.0;
		g[cid].m_cold_z	=	0.0;
		g[cid].z_cold	=	0.0;
	}
	
	g[cid].lastMerge	=	sid;

	return(0);
}

double merge_time(int center, int sat, double r_sat){

	double	m_big, m_small, time;

	int		pid;

	pid	=	g[center].firstProg;
	if(pid == -1){
		r_sat	=	g[center].r_vir;
	}

	m_big	=	g[center].m_vir;
	m_small	=	g[sat].m_vir;

	time	=	tao(r_sat, g[center].v_vir, m_big, m_small);

	return(time);
}

void integrate(int hid){
	double	dm_stellar, dm_eject, dm_cool, dm_cold, dm_hot;
	double	dm_stellar_z, dm_eject_z, dm_cool_z, dm_cold_z, dm_hot_z;
	double	dm_total, dm_total_z;
	
	int		mid;

	dm_stellar		=	0.0;
	dm_stellar_z	=	0.0;
	dm_eject		=	0.0;
	dm_eject_z		=	0.0;
	dm_cool			=	0.0;
	dm_cool_z		=	0.0;
	dm_cold			=	0.0;
	dm_cold_z		=	0.0;
	dm_hot			=	0.0;
	dm_hot_z		=	0.0;

	if(g[hid].type != 2){
		g[hid].sfr	=	sfr(g[hid].v_vir, g[hid].t_dyn, g[hid].m_cold);
	}

	if(g[hid].m_cold < LOWINF){
		goto	TYPE;
	}

	dm_stellar	=	(1.0 - R) * g[hid].sfr * dt;
	dm_eject	=	m_eject(g[hid].v_vir, dm_stellar);
	dm_total	=	dm_stellar + dm_eject;

	if(dm_total > g[hid].m_cold){
		dm_stellar	=	g[hid].m_cold / dm_total * dm_stellar;
		dm_eject	=	g[hid].m_cold - dm_stellar;
	}

	dm_stellar_z	=	dm_stellar * g[hid].z_cold;
	dm_eject_z		=	dm_eject * g[hid].z_cold;
	dm_total_z		=	dm_stellar_z + dm_eject_z;

	if(dm_total_z > g[hid].m_cold_z){
		dm_stellar_z	=	g[hid].m_cold_z / dm_total_z * dm_stellar_z;
		dm_eject_z	=	g[hid].m_cold_z - dm_stellar_z;
	}

	mid	=	g[hid].currentMain;
	g[mid].m_eject_step		+=	dm_eject;
	g[mid].m_eject_z_step	+=	dm_eject_z;

TYPE:
	if(g[hid].type == 0){

		dm_cool	=	g[hid].coolingRate * dt;

		if(dm_cool > g[hid].m_hot){
			dm_cool	=	g[hid].m_hot;
		}

		dm_cold	=	dm_cool - dm_stellar - dm_eject;

		dm_hot	=	-dm_cool + g[hid].m_eject_step;
		g[hid].m_eject_step	=	0.0;

		dm_cool_z	=	dm_cool * g[hid].z_hot;
		if(dm_cool_z > g[hid].m_hot_z){
			dm_cool_z	=	g[hid].m_hot_z;
		}

		dm_cold_z	=	dm_cool_z + p * dm_stellar / (1.0 - R)
						- dm_stellar_z - dm_eject_z;

		dm_hot_z	=	-dm_cool_z + g[hid].m_eject_z_step;
		g[hid].m_eject_z_step	=	0.0;
	
	}else{

		dm_cold		=	-dm_stellar - dm_eject;
		dm_cold_z	=	p * dm_stellar / (1.0 - R)
						- dm_stellar_z - dm_eject_z;
	}

	g[hid].m_stellar	+=	dm_stellar;
	g[hid].m_stellar_z	+=	dm_stellar_z;

	g[hid].m_cold		+=	dm_cold;
	g[hid].m_cold_z		+=	dm_cold_z;
	if(g[hid].m_cold_z > g[hid].m_cold){
		g[hid].m_cold_z	=	g[hid].m_cold;
	}
	if(g[hid].m_cold < LOWINF){
		g[hid].m_cold	=	0.0;
		g[hid].m_cold_z	=	0.0;
		g[hid].z_cold	=	0.0;
	}else{
		g[hid].z_cold	=	g[hid].m_cold_z / g[hid].m_cold;
	}

	if(g[hid].type == 0){
		g[hid].m_hot	+=	dm_hot;
		g[hid].m_hot_z	+=	dm_hot_z;
		if(g[hid].m_hot_z > g[hid].m_hot){
			g[hid].m_hot_z	=	g[hid].m_hot;
		}
		if(g[hid].m_hot < LOWINF){
			g[hid].m_hot	=	0.0;
			g[hid].m_hot_z	=	0.0;
			g[hid].z_hot	=	0.0;
		}else{
			g[hid].z_hot	=	g[hid].m_hot_z / g[hid].m_hot;
		}
	}
}
