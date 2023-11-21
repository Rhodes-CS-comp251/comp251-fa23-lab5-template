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
 * nonblock - a function that makes a file descriptor non-blocking
 * @param fd file descriptor
 */
void nonblock(int fd) {
  int flags;

  if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
    perror("fcntl (get)");
    exit(1);
  }
  if (fcntl(fd, F_SETFL, flags | FNDELAY) == -1) {
    perror("fcntl (set)");
    exit(1);
  }
}

/**
 * get_in_addr - helper function to get pointer to the right address
 * @param sa - IPv4 or IPv6 sockaddr
 * @returns pointer to in_addr
 */
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in *)sa)->sin_addr);
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}


/**
 * pwait - sleeps for a small amount of time before polling
 */
void pwait(void) {
  // avoid true busy waiting
  struct timespec sleep;
  sleep.tv_sec = g.timeout / 1000;               // ms to sec
  sleep.tv_nsec = (g.timeout % 1000) * 1000000L; // ms to nsec
  nanosleep(&sleep, NULL);
}



/**
 * get_status - prints character string for buffer status
 * @param s - buffer status
 * @returns string containing buffer state
 */
char *get_status(status_t s) {
  char *ret = "undefined";
  switch (s) {
    case Pending:
      ret = "pending";
      break;
    case Delivered:
      ret = "delivered";
      break;
    case Unconnected:
      ret = "unconnected";
      break;
    case EndOfFile:
      ret = "EOF";
      break;
  }
  return ret;
}
