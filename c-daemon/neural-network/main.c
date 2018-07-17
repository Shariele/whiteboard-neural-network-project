// http://zetcode.com/db/mysqlc/


#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>									// https://dev.mysql.com/downloads/connector/
#include <string.h>

#define MATRIXSIZE 32

void printError(MYSQL *con){
	fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}


int main(int argc, char const *argv[]){
	int numFields, i, matrix[MATRIXSIZE][MATRIXSIZE], x = 0, y = 0;
	char sql[200];	memset(sql, '\0', sizeof(sql));
	
	// mysql variables
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *con = mysql_init(NULL);					//Initialize variable?
	
	// printf("MYSQL client version: %s\n", mysql_get_client_info());
	

	if(con == NULL) {
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	// Connect to db
	if(mysql_real_connect(
	  	con, 
	  	"localhost", 								//address
	  	"root", 									//Username
	  	"", 										//password
	  	"whiteboard_train_data", 					//db name
	  	3306, 										//Port, 3306 is standard mysql MariaDB i think
	  	"/opt/lampp/var/mysql/mysql.sock", 			//Unix socket
	  	0
	) == NULL)
		printError(con);  
	else
		printf("Alles gucchi\n");

	
	strcpy(sql, 
		"SELECT * FROM matrix INNER JOIN numbers ON matrix.matrix_id = numbers.matrix_id WHERE matrix.matrix_id = 2"
	);
	if(mysql_query(con, sql))
		printError(con);

	result = mysql_store_result(con);
	if(result == NULL)
		printError(con);

	numFields = mysql_num_fields(result);
	for(i = 0; i < MATRIXSIZE; i++)
		printf("%2d ", i);
	printf("\n");


	while((row = mysql_fetch_row(result))){
		for(i = 0; i < numFields; i++){
			// 0: matrix.id
			// 1: col
			// 2: row
			// 3: value
			// 4: numbers.id
			// 5: matrix.id
			// 6: number

			if(i == 3){
				matrix[y][x++] = atoi(row[i]);
				// printf("%2s ", row[i]);


				if(x == MATRIXSIZE){
					y++;
					x = 0;
					// printf("\n");
				}
				
			}
			// printf("%s ", row[i] ? row[i] : "NULL");
		}
		// printf("\n");
	}

	for(y = 0; y < MATRIXSIZE; y++){
		for(x = 0; x < MATRIXSIZE; x++){
			printf("%2d ", matrix[x][y]);
		}
		printf("\n");
	}

	mysql_free_result(result);
	mysql_close(con);
	return 0;
}

