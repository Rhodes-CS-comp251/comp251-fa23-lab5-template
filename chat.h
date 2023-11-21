/**
 * chat.h - header file for chat data structures
 */

#pragma once

#define BUFSIZE       1024
#define MAXCLIENTS      10

/*
 * denotes the state of a given chat buffer:
 *   - Pending     - contains an undelivered messages
 *   - Delivered   - buffer is available for a new message from client
 *   - Unconnected - this conn_t structure is unused
 *   - EndOfFile   - monitor sets this upon EOF, relay sets all status to this to shutdown
 */
typedef enum { Pending, Delivered, Unconnected, EndOfFile } status_t;

/*
 * type used to denote whether program is running as client or server
 */
typedef enum { Client, Server } chatmode_t;

/*
 * main shared memory region for relay
 */
typedef struct conn_s {
  int               id;
  pthread_t         tid;               // thread id for connection thread
  status_t          status;            // message buffer status
  int               sockfd;            // socket file descriptor
  char              message[BUFSIZE];  // message bufer
  int               msgsize;           // message size
  pthread_mutex_t   mutex;             // lock
} conn_t;


/*
 * global variable structure
 */
typedef struct global_s {
  int          verbose;                // print out extra debugging messages
  conn_t       conn[MAXCLIENTS];       // array of connection buffers for clients
  char        *hoststr;                // hostname string (if client mode)
  char        *portstr;                // connect port (if client) / listen port (if server)
  chatmode_t   mode;                   // client or server?
  int          timeout;                // timeout in milliseconds
} global_t;
global_t g;


/*
 * function prototypes
 */

/* util.c */
void  nonblock(int fd);
void *get_in_addr(struct sockaddr *sa);
void  pwait(void);
char *get_status(status_t s);

/* chat.c */
int   client_setup(void);
void  client_chat(int sockfd);
int  server_setup(void);
void *server_chat(void *arg);
void *monitor_chat(void *arg);
void  relay(int sockfd);
