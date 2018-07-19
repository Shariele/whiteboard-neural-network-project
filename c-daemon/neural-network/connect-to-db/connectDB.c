// http://zetcode.com/db/mysqlc/

/**************************************************************************/
/*	INCLUDES	 														  */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************************************/
/*	IMPORT			 													  */
/**************************************************************************/
#include "connectDB.h"

/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define DEBUG 1
#define SQLLENGTH 200


/**************************************************************************/
/*	PRIVATE METHODS														  */
/**************************************************************************/

static void fetchData(MYSQL *con){
	int numFields, i, countTD = 0, x = 0, y = 0, z = 0, doOnceNr = 1, doOnceId = 1;
	char sql[SQLLENGTH];	memset(sql, '\0', sizeof(sql));
	
	// mysql variables
	MYSQL_RES *result;
	MYSQL_ROW row;
				
	
	if(con == NULL) {
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	
	// Create sql. Has to be in "straight line"
	strcpy(sql, 
		"SELECT value, number, matrix.matrix_id FROM matrix INNER JOIN numbers ON matrix.matrix_id = numbers.matrix_id;"
	);
	//  WHERE matrix.matrix_id = 2
	if(mysql_query(con, sql))
		printError(con);

	// Query results
	result = mysql_store_result(con);
	if(result == NULL)
		printError(con);

	// Get number of columns
	numFields = mysql_num_fields(result);

	// Values on i:
	// 0: Values(1 or 0)
	// 1: number
	// 2: matrix.id
	while((row = mysql_fetch_row(result)) && countTD < TRAINDATASIZE){
		for(i = 0; i < numFields; i++){

			if(i == 0){
				trainData[countTD].matrix[y][x++] = atoi(row[i]);

				if(x == MATRIXSIZE && y == MATRIXSIZE-1){			// If Y == 31 and x == 32 it means that whole matrix has been transfered.
					countTD++;										// Reset variables and increment countTD.
					doOnceId = 1;
					doOnceNr = 1;
					y = 0; x = 0;
					break;

				}else if(x == MATRIXSIZE){							// Reset x and increment y 
					y++;
					x = 0;
				}
			}else if(i == 1 && doOnceNr){							//Only do number and id once per matrix to decrease workload
				trainData[countTD].number = atoi(row[i]);
				doOnceNr = 0;
			}else if(i == 2 && doOnceId){
				trainData[countTD].id = atoi(row[i]);
				doOnceId = 0;
			}
		}
	}
	countTD = 0;

	
	
	if(DEBUG){

		// Print matrix
		for(z = 0; z < TRAINDATASIZE; z++){
			printf("\nCountTD: %d\n", countTD);
			printf("\nId for matrix: %d \tNumber for matrix: %d\n", trainData[countTD].id, trainData[countTD].number);
			for(i = 0; i < MATRIXSIZE; i++)
				printf("%2d ", i);
			printf("\n");
			for(y = 0; y < MATRIXSIZE; y++){
				for(x = 0; x < MATRIXSIZE; x++){
					printf("%2d ", trainData[countTD].matrix[x][y]);
				}
				printf("\n");
			}
			countTD++;
			printf("\n");
		}
	}
	
	

	// Free memory of mysql result
	mysql_free_result(result);
	// Close connection
	mysql_close(con);
}


/**************************************************************************/
/*	PUBLIC METHODS														  */
/**************************************************************************/
void getTrainData(MYSQL *con){
	fetchData(con);
}

void printError(MYSQL *con){
	fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}