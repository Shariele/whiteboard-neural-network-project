/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define MATRIXSIZE 32		// The size of our matrices. NEVER CHANGE THIS!
#define TRAINDATASIZE 10	// Defines how many train data to fetch

typedef struct matrices{
	int matrix[MATRIXSIZE][MATRIXSIZE];
	int number;
	int id;
} matrices;