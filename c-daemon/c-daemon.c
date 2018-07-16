#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *socket_path = "/tmp/node-communication";

int main(int argc, char const *argv[]) {
	struct sockaddr_un addr;
	char buf[100], answer[50];
	int fd, cl, rc, stop = 0;

	if (argc > 1) socket_path = argv[1];

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
		perror("socket error");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_UNIX;
	if(*socket_path == '\0'){
		*addr.sun_path = '\0';
		strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
	}else{
		strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
		unlink(socket_path);
	}

	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		perror("bind error");
		exit(-1);
	}

	if(listen(fd, 5) == -1){
		perror("listen error");
		exit(-1);
	}

	printf("Server running...\n");
	printf("Awaiting connection..\n");
	while(1){
		if( (cl = accept(fd, NULL, NULL)) == -1){
			perror("accept error");
			continue;
		}

		// while( (rc = read(cl, buf, sizeof(buf))) > 0 && stop == 0){
		// 	printf("read %u bytes: %.*s\n", rc, rc, buf);
		// 	printf("This was the content recieved: %.*s\n", rc, buf);
		// 	rc = 0;
		// 	break;
		// }
		if((rc = read(cl, buf, sizeof(buf))) > 0){
			printf("read %u bytes: %.*s\n", rc, rc, buf);
			printf("This was the content recieved: %.*s\n", rc, buf);
			rc = 0;
		}

		memset(answer, '\0', sizeof(answer));
		strcpy(answer, "Content recieved, thank you.");

		if((send(cl, &answer, sizeof(answer), 0)) == -1){
			printf("Error when sending datagram\n");
			exit(1);
		}

		if(rc == -1){
			perror("read");
			exit(-1);
		}else if(rc == 0){
			printf("EOF\n");
			close(cl);
		}
	}


	return 0;
}