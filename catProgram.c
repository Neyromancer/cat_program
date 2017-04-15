#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>	// included just in case with stdio.h won't compile
#include <string.h>
#include <ctype.h>

//=============== define terminal output appearance ============
#define BLACK		"\x1B[30m"
#define RED		"\x1B[31m"
#define GREEN		"\x1B[32m"
#define YELLOW		"\x1B[33m"
#define BLUE		"\x1B[34m"
#define MAGENTA		"\x1B[35m"
#define CYAN		"\x1B[36m"
#define WHITE		"\x1B[37m"
//==============================================================

// prototypes
static int displayFileContents( char **const, const int );
static void displayStdInStdOut( void );
static int isOption( char *const );
static int processFile( const char *, char *const, int );
static int readFileContents( FILE *, char *const, int );
static int writeStdOut( char *const, char *const, int );
static int processOption( char *const, int *const, int * );	// process options
static int isOptionAvailable( char );
static void printText( char *const );
//static void setColor( /* int, int, int */ ); this function can be used to set particular text colour

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
	size_t positionOfOptionLine = 0;	// set to 0 as 0 in *argv[] occupied by program name
	int optionEntered = 0;	// 0 stays for no option enetered and 1 - for entered

	// find the position of option line in the squence entered
	for ( size_t i = 1; i < arrSize; ++i )
		if ( isOption( arr[ i ] ) == 1 ) {
			optionEntered = 1;
			positionOfOptionLine = i;
		}

	// check if any file name was entered
	if ( ( arrSize == 2 ) && ( positionOfOptionLine != 0 ) )  {
		puts( "incorrect input. No file entered." );

		return -1;
	}

	// process all entered files
	for ( size_t i = 1; i < arrSize; ++i )
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
   *	+A - display all
   *	+n - number all output lines
   *	-b - number non-blank lines
   *	+e - equivalent to -vE
   *	+E - display $ at the end of each line
   *	-v - show non-printing characters
   *	-s - suppress repeated empty output lines
   *    -d - delete extensive spaces
   *	-t - equivalent to -vT
   *	-T - display TAB characters as ^I
   *	+c - colour every second line, counting from the 1st line
   *    +c[...] - colour every numer line, number set in [...]
   *	
***/	
	static int optionArr[ 77 ] = {};
	static int linesToColor = 2;	// represent which lines to color
//	printf( "in writeStdOut function\n" );
	if ( optionEntered != 0 ) {
		processOption( lineOfOptions, optionArr, &linesToColor );

		static int countLines = 1;	// number displayed in stdout before the line if option_n was set
		if ( 1 == optionArr[ ( 'A' - '-' ) ] ) {
			optionArr[ ( 'n' - '-' ) ] = 1;
			optionArr[ ( 'e' - '-' ) ] = 1;
			optionArr[ ( 's' - '-' ) ] = 1;
			optionArr[ ( 't' - '-' ) ] = 1;
			optionArr[ ( 'c' - '-' ) ] = 1;
		}

		if ( 1 == optionArr[ ( 'e' - '-' ) ] ) {
			optionArr[ ( 'v' - '-' ) ] = 1;
			optionArr[ ( 'E' - '-' ) ] = 1;
		}

		if ( 1 == optionArr[ ( 't' - '-' ) ] ) {
			optionArr[ ( 'v' - '-' ) ] = 1;
			optionArr[ ( 'T' - '-' ) ] = 1;
		}

		if ( 1 == optionArr[ ( 'c' - '-' ) ] && countLines % linesToColor == 0 )
			printf( BLUE );

		if ( 1 == optionArr[ ( 'n' - '-' ) ] ) 
			printf( "%d. ", countLines );
		
		if ( 1 == optionArr[ ( 'E' - '-' ) ] )
			printText( textLine );
		else
			printf( "%s", textLine );

		++countLines;
	} else printf( "%s", textLine );
	printf( WHITE );
}

// check if the entered line is an option
static int isOption( char *const line ) {
	// is not an option, check if the 1st symbol of entered line is '-' which stays for starting an option
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
		default:
			if ( isdigit( ch ) )
				return 1;
	}


	return 0;	// no such option
}

// process options via reference and set if they entered or not
static int processOption( char *const line, int *const optionArr, int *lineToColor ) {
	size_t i = 1;	// not start from 0 as line[ 0 ] occupied with " - "
	do {
		if ( !isdigit( line[ i ] ) )
			optionArr[ ( line[ i ] - '-' ) ] = 1;
		else if ( line[ i - 1 ] == 'c' ) 
			*lineToColor = ( line[ i ] - '0' );

		++i;
	} while ( line[ i ] != '\0' );

	return 0;
}

//================================================== StdIn - StdOut ===================================================//
static void displayStdInStdOut( void ){
	char *line;
	char buffer[ 4096 ];
	size_t i = 0;
	line = ( char * )malloc( sizeof( buffer ) ); // check for errors
	
	while ( i >= 0 ) {
		line[ i ] = getchar();
		printf( "%c", line[ i ] );
		++i;
	}
}

//===================== Print Line Of Text With Special Character At The End Of The Line ==============================//
static void printText( char *const textLine ) {
	textLine[ strlen( textLine ) - 1 ] = '$';
	printf( "%s\n", textLine );
}

/* this function will be used for setting colours
//============================================= Change - Output Text Color ============================================//
static void setColor( / int attr, int bg, int fg / ) {
	
}
*/
