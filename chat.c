#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "chat.h"


/**
 * client_setup - establish TCP connection to chat server
 *
 * @returns socket file descriptor
 */
int client_setup(void) {
  int sockfd = -1;

  // get IP address for host string
  // create a socket
  // connect to server


  // leave these here
  nonblock(STDIN_FILENO);
  nonblock(sockfd);

  return sockfd;
}



/**
 * client_chat - reads messages from stdin/socket and writes to socket/stdout
 *
 * @param sockfd - socket file descriptor
 */
void client_chat(int sockfd) {
  int eof = 0;
  char buf[BUFSIZE];

  while (!eof) {

    // implement me

    // avoid true busy waiting
    pwait();
  }
}



/**
 * server_setup - setup TCP chat listener
 *
 * @returns socket file descriptor
 */
int server_setup(void) {
  struct addrinfo hints, *servinfo;
  int sockfd = -1;
  int val;

  // implement me


  // permit rapid re-use of server port
  val = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
  return sockfd;
}



/*
 * relay - read messages from clients in shared memory, then send to all other clients/monitor
 *
 * @param sockfd - the main listen socket to accept() new connections on
 */
void relay(int sockfd) {
  int eof = 0, i;
  char *fullmsg = "All chat connections are currently in use.\n";

  // initialize all connections

  // implement me

  while (!eof) {

    // implement me

    pwait();
  }

  // got EOF from monitor, shut it down

  // implement me

  // cleanup -- leave here
  for (i=0; i<MAXCLIENTS; i++) {
    pthread_join(g.conn[i].tid, NULL);
    pthread_mutex_destroy(&g.conn[i].mutex);
  }
}



/**
 * server_chat - thread handler for a client connection
 *
 * @param arg - the conn_t structure for this client thread to use
 * @returns NULL
 */
void *server_chat(void *arg) {
  conn_t *conn = (conn_t *)arg;
  int eof = 0;

  if (g.verbose)
    printf("sthread[%d]: starting server thread\n", conn->id);

  while (!eof) {

    // implement me

    pwait();
  }

  if (g.verbose) {
    printf("sthread[%d]: terminating\n", conn->id);
  }

  return NULL;
}



/**
 * monitor_chat - thread handler for a monitor connection
 *
 * @param arg - the conn_t structure for monitor thread to use
 * @returns NULL
 */
void *monitor_chat(void *arg) {
  conn_t *conn = (conn_t *)arg;
  int eof = 0;

  nonblock(STDIN_FILENO);

  if (g.verbose)
    printf("monitor: starting\n");

  while (!eof) {

    // implement me

    pwait();
  }

  if (g.verbose)
    printf("monitor: exiting\n");
  return NULL;
}



int main(int argc, char **argv) {
  int c = -1, sockfd;
  char *portstr = "5000";
  char *hoststr = "localhost";

  setbuf(stdout, NULL);

  // initialize global data structure
  memset(&g, 0, sizeof(g));
  g.portstr = portstr;
  g.hoststr = hoststr;
  g.mode = Server;
  g.timeout = 2000;  // in milliseconds
  //g.timeout = 20;  // use for real

  while ((c = getopt(argc, argv, "ch:p:t:v")) != -1) {
    switch (c) {
      case 'c':
        g.mode = Client;
        break;
      case 'h':
        g.hoststr = optarg;
        break;
      case 'p':
        g.portstr = optarg;
        break;
      case 't':
        g.timeout = atoi(optarg);
        break;
      case 'v':
        g.verbose = 1;
        break;
    }
  }

  if (g.mode == Client) {
    sockfd = client_setup();
    client_chat(sockfd);
  } else {
    sockfd = server_setup();
    nonblock(sockfd);         // listen socket is non-blocking

    // spawn monitor thread
    g.conn[0].status = Delivered; // claim connection 0
    g.conn[0].sockfd = STDOUT_FILENO;
    g.conn[0].id = 0;
    pthread_mutex_init(&g.conn[0].mutex, NULL);
    pthread_create(&g.conn[0].tid, NULL, monitor_chat, &g.conn[0]);

    // invoke relay
    relay(sockfd);

    // after relay exits, ensure monitor terminates
    pthread_join(g.conn[0].tid, NULL);
  }

  return 0;
}
