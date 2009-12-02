#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

#include	"allvars.h"
#include	"proto.h"

int main(int argc, char **argv){

	init();

/*	set open file number	*/

	int	fileNum, treeNum;

	fileNum	=	0;
//	treeNum	=	0;
	
	open_file(fileNum);
	begin_output(fileNum);
	for(treeNum = 0; treeNum < All.Ntrees; ++treeNum){
		read_tree(treeNum);
		construct_tree(treeNum);
		run(treeNum);
		output_tree(treeNum);
	}
	end_output();

	return 0;
}
