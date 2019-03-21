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

	int sock;
	if (sock = socket(AF_INET, SOCK_STREAM, 0) < 0){
		fprintf(stderr, "Error creating socket");
		exit(1);
	}
	int bound = bind(sock, (struct sockaddr *));
	fprintf(stdout, "%d\n", sock);
	return 1;
}