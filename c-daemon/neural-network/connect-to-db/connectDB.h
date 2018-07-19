/**************************************************************************/
/*	INCLUDES	 														  */
/**************************************************************************/
// https://dev.mysql.com/downloads/connector/		Use this flag during compilation for each .c including this file it: `mysql_config --cflags --libs`
#include <mysql.h>	

/**************************************************************************/
/*	IMPORT			 													  */
/**************************************************************************/
#include "globalDef.h"

/**************************************************************************/
/*	PUBLIC METHODS														  */
/**************************************************************************/
// Fills the struct trainData with data from DB
void getTrainData(MYSQL *con);
// Used to print errors
void printError(MYSQL *con);


/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/

struct matrices trainData[TRAINDATASIZE];