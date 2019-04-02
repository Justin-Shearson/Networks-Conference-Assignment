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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#define MAXNAMELEN 256
/*--------------------------------------------------------------------*/


/*----------------------------------------------------------------*/

int startserver() {
  int     sd;        /* socket */
  char serverhost[MAXNAMELEN + 1];  /* hostname */
  ushort  serverport;  /* server port */

  /* Structs */
  struct sockaddr_in server_address;
  struct hostent *host_ent;

  /*
    Creates the socket for the server.
    Returns an error stating that the socket could not be
      created if sd returns -1.
  */
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "Error creating socket");
    close(sd);
    return(-1);
  }

  server_address.sin_family = AF_INET;
  serverport = server_address.sin_port;
  server_address.sin_addr.s_addr = INADDR_ANY;


  /*
    Binds the server socket to a random port
    Returns an error if the socket could not be bound.
  */
  if(bind(sd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
    printf("Could not bind socket with Error: %s\nSocket number: %d\n", strerror(errno), sd);
    close(sd);
    return(-1);
  }
  

  /* ready to receive connections */
  listen(sd, 5);

  /* Gets the hostname and stores it in serverhost. */
  if(gethostname(serverhost, MAXNAMELEN) == -1) {
    printf("Could not get host name: %s\n", strerror(errno));
    close(sd);
    return(-1);
  } else {
    host_ent = gethostbyname(serverhost); //Store the server host information
  }

  /*
    TODO:
    get the port assigned to this server (serverport)
    use getsockname()
  */
  int addrlen = sizeof(server_address);
  if(getsockname(sd, (struct sockaddr *) &server_address, &addrlen) == -1) {
    printf("Could not retrieve socket name: %s\n", strerror(errno));
    close(sd);
    return(-1);
  }

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
int connecttoserver(char *serverhost, ushort serverport) {
  int     sd;          /* socket */
  ushort  clientport;  /* port assigned to this client */
  
  /* Create a TCP socket for the client to use */
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("Error creating socket: %s\n", strerror(errno));
  }

  /* Srtructures */
  struct sockaddr_in server_address, client_address;

  /* Struct used to hold information of the host that the client is connecting to */
  struct hostent *host_ent;
  

  /*
    TODO:
    connect to the server on 'serverhost' at 'serverport'
    use gethostbyname() and connect()
  */
  /* Gets the host name and stores it in the host_ent struct. */
  host_ent = gethostbyname(serverhost);
  if(host_ent == NULL) {
    printf("Host name does not exist\n");
    close(sd);
    exit(1);
  }

  /* Assigning attributes for the server struct */
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  /* Copy over the address from host_ent to s_addr in server address */
  bcopy((char *) host_ent->h_addr, (char *) &server_address.sin_addr.s_addr, host_ent->h_length);
  server_address.sin_port = htons(serverport);
  /* Assigning attributes for the client struct */
  client_address.sin_family = AF_INET;
  client_address.sin_addr.s_addr = INADDR_ANY;
  clientport = client_address.sin_port;

  if(connect(sd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
    printf("There was a problem connecting to the server: %s\n", strerror(errno));
  }

  
  /*
    TODO:
    get the port assigned to this client
    use getsockname()
  */
  int addrlen = sizeof(server_address);
  if(getsockname(sd, (struct sockaddr *) &server_address, &addrlen) == -1) {
    printf("Could not retrieve socket name %s\n", strerror(errno));
    close(sd);
    exit(1);
  }

  /* succesful. return socket */
  printf("admin: connected to server on '%s' at '%hu' thru '%hu'\n",
	 serverhost, serverport, clientport);
  return(sd);
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
int readn(int sd, char *buf, int n) {
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

char *recvdata(int sd) {
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

int senddata(int sd, char *msg) {
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
