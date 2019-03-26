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

extern char *  recvdata(int sd);
extern int     senddata(int sd, char *msg);

extern int     startserver();
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/* main function*/
main(int argc, char *argv[])
{
  int    serversock;    /* server socket*/

  fd_set liveskset;   /* set of live client sockets */
  int    liveskmax;   /* maximum socket */

  /* check usage */
  if (argc != 1) {
    fprintf(stderr, "usage : %s\n", argv[0]);
    exit(1);
  }

  /* get ready to receive requests */
  serversock = startserver();
  if (serversock == -1) {
    exit(1);
  }
  
  /*
    TODO:
    init the live client set 
  */

  /* receive and process requests */
  while (1) {
    int    itsock;      /* loop variable */

    /*
      TODO:
      using select() to serve both live and new clients
    */
    
    /* process messages from clients */
    for (itsock=3; itsock <= liveskmax; itsock++) {
      /* skip the listen socket */
      if (itsock == serversock) continue;

      if ( /* TODO: message from client */ ) {
	char *  clienthost;  /* host name of the client */
	ushort  clientport;  /* port number of the client */
	
	/*
	  TODO:
	  obtain client's host name and port
	  using getpeername() and gethostbyaddr()
	*/
	
	/* read the message */
	msg = recvmsg(itsock);
	if (!msg) {
	  /* disconnect from client */
	  printf("admin: disconnect from '%s(%hu)'\n",
		 clienthost, clientport);

	  /*
	    TODO:
	    remove this client from 'liveskset'  
	  */

	  close(itsock);
	} else {
	  /*
	    TODO:
	    send the message to other clients
	  */
	  
	  /* print the message */
	  printf("%s(%hu): %s", clienthost, clientport, msg);
	  free(msg);
	}
      }
    }

    if ( /* TODO: connect request from a new client */ ) {

      /*
	TODO:
	accept a new connection request
      */

      if (/* TODO: if accept is fine */) {
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
      } else {
	perror("accept");
	exit(0);
      }
    }
  }
}
/*--------------------------------------------------------------------*/
