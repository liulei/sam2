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

double	calc_hubble(double z){

	double	a, h;

	a	=	1.0 / (1.0 + z);

	h	=	1.0 - Omega_m + Omega_m / (a * a * a);

	h	=	All.Hubble * sqrt(h);

	return(h);
}

double	r_vir(float Mass, float z){

	double	h, r;

	h	=	calc_hubble(z);

	r	=	pow(All.G * Mass / (100.0 * h * h), 1.0 / 3.0);

	return(r);
}

double	v_vir(float Mass, float z){
	
	double	h, v;
	
	h	=	calc_hubble(z);

	v	=	pow(10.0 * All.G * Mass * h, 1.0 / 3.0);

	return(v);
}

double	t_dyn(float Mass, float z){
	
	double	h, t;

	h	=	calc_hubble(z);

	t	=	0.01 / h;

	return(t);
}

double	sfr(double velocity, double t_dyn, double m_cold){

	double	alpha, m_crit, rate;
	
	alpha	=	ALPHA_0 * pow(velocity / 300.0, NSFR);
	if(alpha >= 1.0){
		alpha	=	1.0;
	}

	m_crit	=	0.0;

	rate	=	alpha * (m_cold - m_crit) / t_dyn;

	if(rate < 0.0)
		rate	=	0.0;

	return(rate);
}
