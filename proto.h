#ifndef	ALLVARS_H
#include	"allvars.h"
#endif

/* init.c: */

void init();
void set_units();
void set_snap_time();

/* aux.c: */
double lookback(double z);

/* io.c */
void open_file(int fileNum);

/* tree.c */
void read_tree(int treeNum);
void construct_tree(int treeNum);

