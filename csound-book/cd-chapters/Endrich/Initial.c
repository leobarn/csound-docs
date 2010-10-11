#include <stdio.h>  
#include <stdlib.h>
#include "cscore.h" 

void	phelp();

void
initial(int argc, char *argv[])		/* Take command line, open files and */
{					/*	deal with switches	*/
	char *ifile, *ofile;

	if(argc > 1 && argv[1][0] == '-') {
		switch(argv[1][1]){
		case 'h':
			phelp();
			exit(0);
		default:
			fprintf(stderr, "switch '%s' not understood - ignored\n", argv[1]);
		}
		argc--, argv++;
	}

	switch(argc) {
	case 2:
		ifile = 0;
		ofile = argv[1];
		break;
	case 3:
		ifile = argv[1];
		ofile = argv[2];
		break;
	default:
		fprintf(stderr, "too many filenames\n");
		/* FALLTHROUGH */
	case 1:
		phelp();
		exit(0);
	}

	if(ifile != 0) {
		if((scfp = fopen(ifile,"r")) == NULL) {
			printf ("Unable to open %s for input\n", ifile);
			exit (-1);
		}
	}

	if(ofile != 0) {
		if(freopen(ofile,"w", stdout) == NULL) {
			printf ("Unable to create %s for output\n", ofile);
			exit (-1);
		}
	}
}

void
phelp(void)			/* print helpful message about parameters */
{
	printf("\nExamples\n");
	printf("\tprogram mynew.sc\t\t[no input allowed, output to file]\n");
	printf("\tprogram myold.sc mynew.sc\t[input from file,  output to file]\n");
}