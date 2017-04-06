#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// prototypes
static int displayFileContents( char **const, const int );
static void displayStdInStdOut( void );
static int isOption( char *const );
static int processFile( const char *, char *const, int );
static int readFileContents( FILE *, char *const, int );
static int writeStdOut( char *const, char *const, int );
static int processOption( char *const, int *const );	// process options
static int isOptionAvailable( char );

// process any stdin fed
int processInput( int argc, char **const argv) {
	if ( argc > 1 )	{	// read and process files
		if ( displayFileContents( argv, argc ) < 0 )
			return -1;
	}
	else 
		 displayStdInStdOut();
}
//=========================================== Process Input Sequence =======================================//
/***
   * Check if options were entered and which options were entered.
***/
static int displayFileContents( char **const arr, const int arrSize ) {	
	int positionOfOptionLine = 0;	// set to 0 as 0 in *argv[] occupied by program name
	int optionEntered = 0;	// 0 stays for no option enetered and 1 - for entered

	// find the position of option line in the squence entered
	for ( int i = 1; i < arrSize; ++i )
		if ( isOption( arr[ i ] ) == 1 ) {
			optionEntered = 1;
			positionOfOptionLine = i;
		}

	// check if any file name was entered
	if ( ( arrSize == 2 ) && ( positionOfOptionLine != 0 ) )  {
		printf( "incorrect input. No file entered." );

		return -1;
	}

	// process all entered files
	for ( int i = 1; i < arrSize; ++i )
		if ( i != positionOfOptionLine )
			if ( processFile( arr[ i ], arr[ positionOfOptionLine ], optionEntered ) < 0 )
				return -1;
}

//=========================================== File Processing ==============================================//

// open file and call function to read from it ( readFileContents )
static int processFile( const char *fileName, char *const lineOfOptions, int optionEntered ) {
	// open file
	FILE *fp = fopen( fileName, "r" );
	if ( NULL == fp ) {
		perror( "Error" );

		return -1;
	}

	if ( readFileContents( fp, lineOfOptions, optionEntered ) < 0 ) {
		printf( "can't read the contents of the file %s\n", fileName );

		return -1;
	}

	if ( fclose( fp ) ) {
		printf( "file %s was corrupted and can't be closed properly\n", fileName );

		return -1;
	}
}

// read file contents and call to writeStdOut function
static int readFileContents( FILE *fileName, char *const lineOfOptions, int optionEntered ) {
	char *textLine;
	char buffer[ 4096 ];
	textLine = ( char * )malloc( sizeof( buffer ) );	// how to allocate enough memory for a string of unknown length
	// check again for errors of not allocating memory
	if ( !textLine ) {
		printf( "not enough memory space to process the file\nFile will be colsed\n" );

		return -1;
	}

	if ( ferror( fileName ) )
		printf( "Error occured while reading from file" );
		
	 while( fgets( textLine, sizeof( buffer ), fileName ) != NULL ) {
//		printf( "%s", line );

		writeStdOut( textLine, lineOfOptions, optionEntered );

	 }
//	 printf( "\n" );

	free( textLine );
	textLine = 0;
}

static int writeStdOut( char *const textLine, char *const lineOfOptions, int optionEntered ) {
/***	
   *	-A - display all
   *	-n - number all output lines
   *	-b - number non-blanl lines
   *	-e - equivalent to -vE
   *	-E - display $ at end of each line
   *	-v - show non-printing characters
   *	-s - suppress repeated empty output lines
   *	-t - equivalent to -vT
   *	-T - display TAB characters as ^I
   *	-c - colout every second line, counting from the 1st line
   *	
***/	
	static int optionArr[ 77 ] = {};
//	printf( "in writeStdOut function\n" );
	if ( optionEntered != 0 ) {
		processOption( lineOfOptions, optionArr );

		static int countLines = 1;	// number displayed in stdout before the line if option_n was set
		if ( optionArr[ ( 'A' - '-' ) ] == 1 ) {
			optionArr[ ( 'n' - '-' ) ] = 1;
			optionArr[ ( 'e' - '-' ) ] = 1;
			optionArr[ ( 's' - '-' ) ] = 1;
			optionArr[ ( 't' - '-' ) ] = 1;
			optionArr[ ( 'c' - '-' ) ] = 1;
		}
//		printf( "OptionEntered\n" );
		if ( optionArr[ ( 'n' - '-' ) ] == 1 ) {
			printf( "%d. ", countLines );
			++countLines;
		}
		printf( "%s", textLine );
		if ( optionArr[ ( 'E' - '-' ) ] == 1 )
			printf( "$\n" );

	} else printf( "%s", textLine );
}

// check if the entered line is an option

static int isOption( char *const line ) {
	// is not an option check if the 1st symbol of entered line is '-' which stays for starting an option
	int ifOption = 0; 
	if ( line[ 0 ] != '-' )	
		return ifOption;

	int i = 1;
	while ( line[ i ] != '\0' ) {
		if ( isOptionAvailable( line[ i ] ) != 1 ) // entered line is not an option line
			return ifOption;
		++i;
	}
	ifOption = 1;	// option was entered
/*
	if ( line[ 0 ] == '-' ) 
		ifOption = 1;	// option
*/
	return ifOption;

}

// check if there is such an option
static int isOptionAvailable( char ch ) {
/***	
   *	-A - display all
   *	-n - number all output lines
   *	-b - number non-blanl lines
   *	-e - equivalent to -vE
   *	-E - display $ at end of each line
   *	-v - show non-printing characters
   *	-s - suppress repeated empty output lines
   *	-t - equivalent to -vT
   *	-T - display TAB characters as ^I
   *	-c - colout every second line, counting from the 1st line
   *	
***/	
	switch( ch ) {
		case 'A':
		case 'n':
		case 'b':
		case 'E':
		case 'e':
		case 'v':
		case 's':
		case 't':
		case 'T':
		case 'c':
			return 1;	// option found
	}

	return 0;	// no such option
}

// process options via reference and set if they entered or not
static int processOption( char *const line, int *const optionArr ) {
	int i = 1;	// not start from 0 as line[ 0 ] occupied with " - "
	do {
		switch( line[ i ] ) {
			case 'n':
				optionArr[ ( 'n' - '-' ) ] = 1;
				break;

			case 'b':
				optionArr[ ( 'b' - '-' ) ] = 1;
				break;

			case 'E':
				optionArr[ ( 'E' - '-' ) ] = 1;
				break;

			case 's':
				optionArr[ ( 's' - '-' ) ] = 1;
				break;

			case 'c':
				optionArr[ ( 'c' - '-' ) ] = 1;
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
