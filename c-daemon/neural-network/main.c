/**************************************************************************/
/*	INCLUDES	 														  */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/**************************************************************************/
/*	IMPORT METHODS	 													  */
/**************************************************************************/
#include "connectDB.h"


/**************************************************************************/
/*	MAIN PROGRAM	 													  */
/**************************************************************************/
int main(int argc, char const *argv[]){
	MYSQL *con = mysql_init(NULL);	


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
	else{
		printf("DB connection is alles gucchi\n");
		getTrainData(con);
	}
	
	return 0;
}