#ifndef	PARAMETER_H
#define	PARAMETER_H

#define		h			0.73
#define		Omega_m		0.25
#define		Omega_b		0.045
#define		f_b			0.17

#define		startNum	9
#define		sqrt_2		1.414
#define		SQRT_H		0.8544
#define		Nstep		20
#define		AGE			9.91497
#define		MC			0.3
//#define		MAXVEL		390.0
#define		MAXVEL		500

#define		ENDSNAP		63
#define		NIL			-6.0
#define		OUTSNAP		63
#define		LOWINF		1.0e-8
#define		Z_SUN		-1.79588
//#define		Z_SUN		1.79588

#ifdef		SALPETER
#define		p			0.032
#define		R			0.35
#define		ALPHA_0		0.10
#define		EPSILON		0.1
//#define		EPSILON		0.4
#define		n			2.2
#define		ETA_SN		6.3
#endif

#ifdef		SCALO
#define		p			0.04
#define		R			0.4
#define		ALPHA_0		0.10
#define		EPSILON		0.2
#define		n			2.2
#define		ETA_SN		5.0
#endif

#endif
