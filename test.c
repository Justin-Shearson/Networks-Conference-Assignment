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
#include <unistd.h>
#include <errno.h>


int main(int argc, char *argv[]){
	
	int sock;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "Error creating socket");
		exit(1);
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
		fprintf(stdout, "%ld\n", sizeof(serv_addr));
		printf("Could not bind socket with Error: %s\nSocket number: %d\n", strerror(errno), sock);
		exit(1);
	}
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(serv_addr.sin_addr), str, INET_ADDRSTRLEN);
	printf("Server Port Number: %d\nServer Address: %s\n", serv_addr.sin_port, str);
	close(sock);

	listen(sock, 5);
	return 1;
}
