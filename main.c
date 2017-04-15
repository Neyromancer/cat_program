// main.c
/***
   * This is a studying project.
   * Program trying to simulate
   * standart cat program
***/
#include <stdio.h>
#include <stdlib.h>

#include "catProgram.h"

int main( int argc, char *argv[] ) {
	if ( processInput( argc, argv ) < 0 )
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

