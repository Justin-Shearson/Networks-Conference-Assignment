/*--------------------------------------------------------------------*/
/* conference server */

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
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

extern char *  recvdata(int sd);
extern int     senddata(int sd, char *msg);

extern int     startserver();
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/* main function*/
int main(int argc, char *argv[]) {
  int    serversock;    /* server socket*/

  fd_set liveskset;   /* set of live client sockets */
  int    liveskmax;   /* maximum socket */
  
  /* Structures */
  struct sockaddr_in server_address, client_address;
  struct in_addr clientname;
  struct timeval timeout;
  timeout.tv_sec = 1;

  /* check usage */
  if (argc != 1) {
    fprintf(stderr, "usage : %s\n", argv[0]);
    exit(1);
  }

  /* get ready to receive requests */
  serversock = startserver();
  if (serversock == -1) {
  	printf("Failed to startserver\n");
  	close(serversock);
    exit(1);
  }
  
  /*
    TODO:
    init the live client set 
  */
  FD_ZERO(&liveskset);
  FD_SET(serversock, &liveskset);
  FD_SET(0, &liveskset);
  /* receive and process requests */
  while (1) {

    int    itsock;      /* loop variable */

    /*
      TODO:
      using select() to serve both live and new clients
    */
    int err = select(liveskmax + 1, &liveskset, NULL, NULL, NULL);
    printf("%d\n", liveskmax);

    if(err == -1){
    	printf("There was an error selecting the client");
    }
    
    /* process messages from clients */
    for (itsock=3; itsock <= liveskmax; itsock++) {
      /* skip the listen socket */
      if (itsock == serversock) continue;

      if (!(err == -1)) {
		char *  clienthost;  /* host name of the client */
		ushort  clientport;  /* port number of the client */
	
	/*
	  TODO:
	  obtain client's host name and port
	  using getpeername() and gethostbyaddr()
	*/
      	socklen_t addresssize = sizeof(server_address);
     	err = getpeername(serversock, (struct sockaddr *) &server_address, &addresssize);
     	if(err == -1){
     		printf("There was an error getting the peername\n");
     	}

     	gethostbyaddr((char *) &server_address.sin_addr, addresssize, AF_INET);

	/* read the message */
	char * msg = recvdata(itsock);
	if (!msg) {
	  /* disconnect from client */
	  printf("admin: disconnect from '%s(%hu)'\n",
		 clienthost, clientport);

	  /*
	    TODO:
	    remove this client from 'liveskset'  
	  */
	  FD_CLR(itsock, &liveskset);
	  close(itsock);
	} else {
	  /*
	    TODO:
	    send the message to other clients
	  */
	  senddata(itsock, msg);
	  
	  /* print the message */
	  printf("%s(%hu): %s", clienthost, clientport, msg);
	  free(msg);
	}
      }
    }

    if (FD_ISSET(serversock, &liveskset)) {

      /*
	TODO:
	accept a new connection request
      */
    	socklen_t clientsize = sizeof(client_address);
    	err = accept(serversock, (struct sockaddr *) &client_address, &clientsize);
      if (!(err == -1)) {
	char *  clienthost;  /* host name of the client */
	ushort  clientport;  /* port number of the client */

	/*
	  TODO:
	  get client's host name and port using gethostbyaddr() 
	*/
	printf("admin: connect from '%s' at '%hu'\n",
	       clienthost, clientport);

	/*
	  TODO:
	  add this client to 'liveskset'
	*/
	FD_SET(itsock, &liveskset);
      } else {
	perror("accept");
	exit(0);
      }
    }
  }
}
/*--------------------------------------------------------------------*/
