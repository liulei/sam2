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
double v_vir(float Mass, float z);
double t_dyn(float Mass, float z);
double sfr(double velocity, double t_dyn, double m_cold);

/* io.c */
void open_file(int fileNum);

/* tree.c */
void read_tree(int treeNum);
void construct_tree(int treeNum);

/* galaxy.c */
void run(int treeNum);
void create_galaxy(int hid);
void evolve_galaxy(int snapNum);
void evolve_single_galaxy(int hid);
void update_center_hot(int hid);
int	construct_galaxy(int hid, int snapNum);
int	halo_to_sat(int pid, int center, int currentMain);
int update_halo_galaxy(int hid);
int update_all_sat(int pid, int center, int currentMain, int isUpdate, double r_sat);
void update_single_sat(int pid, int center, int currentMain, int isUpdate, double r_sat);
int merge(int sid);
void integrate(int hid);
int jump_galaxy(int pid);

/* cooling.c */
void init_cooling();
double cooling_rate(int hid);
double cooling_radius(int hid);
double lambda(double z_hot, double T);
double search_row(int idz, double logT);
