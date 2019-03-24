#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>
#include <time.h> 
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	
	struct sockaddr_in serv_addr;
	int sock;
	if (sock = socket(AF_INET, SOCK_STREAM, 0) < 0){
		fprintf(stderr, "Error creating socket");
		exit(1);
	}
int portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr) < 0)){
		herror("ERROR binding socket");
	}
	fprintf(stdout, "%d\n", sock);
	return 1;
}