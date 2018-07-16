

// https://dev.mysql.com/downloads/connector/
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>



int main(int argc, char const *argv[]){
	
	printf("MySQL client version: %s\n", mysql_get_client_info());
	 MYSQL *con = mysql_init(NULL);

	  if (con == NULL) 
	  {
	      fprintf(stderr, "%s\n", mysql_error(con));
	      exit(1);
	  }

	  if (mysql_real_connect(con, "localhost", "root", "", 
	          "whiteboard_train_data", 3306, "/opt/lampp/var/mysql/mysql.sock", 0) == NULL) 
	  {
	      fprintf(stderr, "%s\n", mysql_error(con));
	      mysql_close(con);
	      exit(1);
	  }  else
	  	printf("Alles gucchi\n");

	  mysql_close(con);

	return 0;
}

