#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// prototypes
static int displayFileContents( char **const, const int );
static void displayStdInStdOut( void );
static int isOption( char *const );
static int processFile( const char *, char *const );
static int readFileContents( FILE *, char *const );
static int writeStdOut( char *const, char *const );
static int processOption( char *const, int *, int *, int *, int * );	// process options

// process any stdin feed
int processInput( int argc, char **const argv) {
	if ( argc > 1 )	{	// read and process files
		if ( displayFileContents( argv, argc ) < 0 )
			return -1;
	}
	else 
		 displayStdInStdOut();
}
//=========================================== Process initial Input =======================================//
/***
   * Check if options were entered and wich options were entered.
***/
static int displayFileContents( char **const arr, const int arrSize ) {	
	int positionOfOptionLine = 0;	// set to 0 as 0 in *argv[] occupied by program name
	// find the option line
	for ( int i = 1; i < arrSize; ++i )
		if ( isOption( *arr[ i ] ) ==  0 )
			positionOfOptionLine = i;

	if ( ( argc == 2 ) && ( positionOfOptionLine == 0 ) ) {
		printf( "incorrect input. No file entered." );

		return -1;
	}
	for ( int i = 1; i < arrSize; ++i )
		if ( i != positionOfOptionLine )
			if ( processFile( *arr[ i ] ) < 0 )
				return -1;
}

//=========================================== File Processing ==============================================//

// open file and call to readFileContents
static int processFile( const char *fileName, char *const lineOfOtions ) {
	FILE *fp = fopen( fileName, "r" );
	if ( fp == NULL ) {
		perror( "Error" );

		return -1;
	}

	if ( readFileContents( fp, lineOfOptions ) < 0 ) {
		printf( "can't read the contents of the file %s\n", fileName );

		return -1;
	}

	if ( fclose( fp ) ) {
		printf( "file %s was corrupted and can't be closed properly\n", fileName );

		return -1;
	}
}

// read file contents and call to writeStdOut function
static int readFileContents( FILE *fileName, char *const lineOfOptions ) {
	char *line;
	char buffer[ 4096 ];
	line = ( char * )malloc( sizeof( buffer ) );	// how to allocate enough memory for a string of unknown length
	if ( !line ) {
		printf( "not enough memory space to process the file\nFile will be colsed\n" );

		return -1;
	}

	if ( ferror( fileName ) )
		printf( "Error occured while reading from file" );
		
	 while( fgets( line, sizeof( buffer ), fileName ) != NULL ) {
//		printf( "%s", line );

		writeStdOut( textLine, lineOfOptions ) {

	 }
//	 printf( "\n" );

	free( line );
}

static int writeStdOut( char *const textLine, char *const lineOfOptions ) {
	static int option_n_b = 0;	// number nonempty output lines
	static int option_E = 0;	// display $ at end of each line
	static int option_s = 0;	// suppress repeated empty output lines
	static int option_c = 0;	// colout every second line, counting from the 1st line
	processOption( lineOfOptions, &option_n_b, &option_E, &option_s, &option_c );

	static int countLines = 1;	// number displayed in before the line
	if ( option_n_b == 1 )
		printf( "%d. ", countLines );
	printf( "%s", textLine );
	if ( option_E == 1 )
		printf( "$\n" );
}

// check if the entered line is an option
static int isOption( char *const line ) {
	int ifOption = 0;	// is not an option
	if ( line[ 0 ] == '-' ) 
		ifOption = 1;	// option

	return ifOption;
}

// process options via reference and set if they entered or not
static int processOption( char *const line, int *option_n_b, int *option_E, int *option_s, int *option_c ) {
	int i = 1;	// not start from 0 as lin[ 0 ] occupied with " - "
	do {
		switch( line[ i ] ) {
			case 'n':
			case 'b':
				*option_n_b = 1;
				break;
			case 'E':
				*option_E = 1;
				break;
			case 's':
				*option_s = 1;
				break;
			case 'c':
				*option_c = 1;
				break;
			default:
				printf( "Error occured: -- %c no such option", line[ i ] );
				return -1;

		}
		++i;
	} while ( line[ i ] != '\0' );

	return 0;
}

//================================================== StdIn - StdOut ===================================================//
static void displayStdInStdOut( void ){
	char *line;
	char buffer[ 4096 ];
	int i = 0;
	line = ( char * )malloc( sizeof( buffer ) ); // check for mistakes
	
	while ( i >= 0 ) {
		line[ i ] = getchar();
		printf( "%c", line[ i ] );
		++i;
	}
}
