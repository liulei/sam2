#ifndef	ALLVARS_H
#include	"allvars.h"
#endif

/* init.c: */

void init();
void set_units();
void set_snap_time();

/* aux.c: */
double lookback(double z);
double calc_hubble(double z);
double r_vir(float Mass, float z);

/* io.c */
void open_file(int fileNum);

/* tree.c */
void read_tree(int treeNum);
void construct_tree(int treeNum);

/* galaxy.c */
void run(int treeNum);
void create_galaxy(int hid);
void evolve_galaxy(int snapNum);
void update_center_hot(int hid);

