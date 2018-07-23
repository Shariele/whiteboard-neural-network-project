/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define MATRIXSIZE 32		// The size of our matrices. NEVER CHANGE THIS!
#define TRAINDATASIZE 10	// Defines how many train data to fetch
#define MNISTSIZE 28		// Temporary 

typedef struct matrices{
	int matrix[MATRIXSIZE][MATRIXSIZE];
	int number;
	int id;
} matrices;