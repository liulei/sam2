#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	"allvars.h"
#include	"proto.h"

static int	it;

static double	dt;
static double	dz;

static double	t_age;
static double	redshift;

void run(int treeNum){
	
	int	i, hid, snapNum;

	g	=	(GALAXY *)malloc(sizeof(GALAXY) * All.treeTable[treeNum]);

	printf("section %d: \n", treeNum);

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

	for(snapNum = STARTSNAP + 1; snapNum <= ENDSNAP; ++snapNum){
		
		evolve_galaxy(snapNum);

		for(i = 0; i < halo_per_snap[snapNum]; ++i){
			hid	=	snap[snapNum][i].hid;
			construct_galaxy(hid);
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

	if(g[hid].type	!=	0){
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
		t_age		=	it * dt + AGE - snap_lookback[snapNum - 1];
		redshift	=	snap_redshift[snapNum - 1] - (it - 1) * dz;
		
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
