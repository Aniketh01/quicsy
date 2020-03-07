#include <stdlib.h> // for basic memmory allocation and deallocation
#include <stdio.h>	// for file read and write
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define MAX 1024
#define PORT 8080

void sentFile(int sockfd)
{
	char buff[MAX]; // for read operation from file and used to sent operation

	// create file
	int send_fd, read_len;
	send_fd = open("/home/devbox/devel/quic_research/datasets/bbb_sunflower_1080p_60fps_normal.mp4", O_RDONLY); // open file uses both stdio and stdin header files

	// file should be present at the program directory

	if (!send_fd)
	{
		printf("Error IN Opening File .. \n");
		return;
	}

	while (1)
	{
		memset(buff, 0x00, MAX);
		read_len = read(send_fd, buff, MAX);
		send(sockfd, buff, read_len, 0);
		if (read_len == 0)
		{
			break;
		}
	}

	printf("File Sent successfully !!! \n");
}

int main()
{
	int sockfd, connfd, len;		  // create socket file descriptor
	struct sockaddr_in servaddr, cli; // create structure object of sockaddr_in for client and server

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // creating a TCP socket ( SOCK_STREAM )

	if (sockfd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		printf("setsockopt(SO_REUSEADDR) failed");

	// empty the
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;				  // specifies address family with IPv4 Protocol
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // binds to any address
	servaddr.sin_port = htons(PORT);			  // binds to PORT specified

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0)
	{
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");

	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len); // accepts connection from socket

	if (connfd < 0)
	{
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");

	// Function for chatting between client and server
	sentFile(connfd);

	// After transfer close the socket
	close(sockfd);
}