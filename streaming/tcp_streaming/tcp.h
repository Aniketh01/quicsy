#ifndef TCP_H
#define TCP_H

// Headers required by client
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

// Headers required by server
#include <netinet/in.h>
#include <sys/types.h>

#define MAX 1024
#define PORT 8080

// TCP client API
int run_tcp_client(const char *filename, const char *infilename);

#endif