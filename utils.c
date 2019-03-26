/*--------------------------------------------------------------------*/
/* functions to connect clients and server */

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h>
#include <time.h> 
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXNAMELEN 256
/*--------------------------------------------------------------------*/


/*----------------------------------------------------------------*/
int startserver()
{
  int     sd;        /* socket */

  char *  serverhost;  /* hostname */
  ushort  serverport;  /* server port */

  /*
    Creates the socekt for the server.
    Returns an error stating that the socket could not be
      created if sd returns -1.
  */
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    fprintf(stderr, "Error creating socket");
    exit(1);
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = serverport;
  server_address.sin_addr.s_addr = INADDR_ANY;
  
  if(bind(sd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1){
    printf("Could not bind socket with Error: %s\nSocket number: %d\n", strerror(errno), sd);
  }
  

  /*
    TODO:
    bind the socket to some random port, chosen by the system 
  */

  /* ready to receive connections */
  listen(sd, 5);

  /*
    TODO:
    obtain the full local host name (serverhost)
    use gethostname() and gethostbyname()
  */

  /*
    TODO:
    get the port assigned to this server (serverport)
    use getsockname()
  */

  /* ready to accept requests */
  printf("admin: started server on '%s' at '%hu'\n",
	 serverhost, serverport);
  return(sd);
}
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/*
  establishes connection with the server
*/
int connecttoserver(char *serverhost, ushort serverport)
{
  int     sd;          /* socket */

  ushort  clientport;  /* port assigned to this client */

  /*
    TODO:
    create a TCP socket 
  */

  /*
    TODO:
    connect to the server on 'serverhost' at 'serverport'
    use gethostbyname() and connect()
  */
  
  /*
    TODO:
    get the port assigned to this client
    use getsockname()
  */

  /* succesful. return socket */
  printf("admin: connected to server on '%s' at '%hu' thru '%hu'\n",
	 serverhost, serverport, clientport);
  return(sd);
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
int readn(int sd, char *buf, int n)
{
  int     toberead;
  char *  ptr;

  toberead = n;
  ptr = buf;
  while (toberead > 0) {
    int byteread;

    byteread = read(sd, ptr, toberead);
    if (byteread <= 0) {
      if (byteread == -1)
	perror("read");
      return(0);
    }
    
    toberead -= byteread;
    ptr += byteread;
  }
  return(1);
}

char *recvdata(int sd)
{
  char *msg;
  long  len;
  
  /* get the message length */
  if (!readn(sd, (char *) &len, sizeof(len))) {
    return(NULL);
  }
  len = ntohl(len);

  /* allocate memory for message */
  msg = NULL;
  if (len > 0) {
    msg = (char *) malloc(len);
    if (!msg) {
      fprintf(stderr, "error : unable to malloc\n");
      return(NULL);
    }

    /* read the message */
    if (!readn(sd, msg, len)) {
      free(msg);
      return(NULL);
    }
  }
  
  return(msg);
}

int senddata(int sd, char *msg)
{
  long len;

  /* write lent */
  len = (msg ? strlen(msg) + 1 : 0);
  len = htonl(len);
  write(sd, (char *) &len, sizeof(len));

  /* write message data */
  len = ntohl(len);
  if (len > 0)
    write(sd, msg, len);
  return(1);
}
/*----------------------------------------------------------------*/
