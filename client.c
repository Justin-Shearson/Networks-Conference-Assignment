/*--------------------------------------------------------------------*/
/* conference client */

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
#include <sys/time.h>

#define MAXMSGLEN  1024

extern char *  recvdata(int sd);
extern int     senddata(int sd, char *msg);

extern int     connecttoserver(char *servhost, ushort servport);
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  int  sock;
  int err;  //Used to return errors

  /* Structures */
  fd_set client_fds;

  /* Timeout Value */
  struct timeval timeout;
  timeout.tv_sec = 2;

  /* check usage */
  if (argc != 3) {
    fprintf(stderr, "usage : %s <servhost> <servport>\n", argv[0]);
    exit(1);
  }

  /* connect to the server */
  sock = connecttoserver(argv[1], atoi(argv[2]));
  if (sock == -1)
    exit(1);

  /* Zeroing the set and adding the client fd to the fd_set */
  FD_ZERO(&client_fds);
  FD_SET(sock, &client_fds);
  FD_SET(0, &client_fds);

  while (1) {
    /* Watch for file descriptors from the server as well as other clients. */
    err = select(sock + 1, &client_fds, NULL, NULL, &timeout);
    if (!(err == -1)) {
      char *msg;
      msg = recvdata(sock);
      if (!msg) {
	/* server died, exit */
	fprintf(stderr, "error: server died\n");
	exit(1);
      }

      /* print the message */
      printf(">>> %s", msg);
      free(msg);
    } else {
      printf("There was an error getting messages from the server: %s\n", strerror(errno));
    }
    /* If there is a message to be sent to the server */
    if ((FD_ISSET(0, &client_fds))) {
      char      msg[MAXMSGLEN];

      if (!fgets(msg, MAXMSGLEN, stdin))
	exit(0);
      senddata(sock, msg);
    }
  }
  return 1;
}
/*--------------------------------------------------------------------*/
