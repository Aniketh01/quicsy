#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080

void recvFile(int sockfd)
{
	char buff[MAX]; // to store message from client

	int recv_fd, read_len;
	recv_fd = open("recieved1.mp4", O_WRONLY | O_CREAT | O_TRUNC,
				   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // stores the file content in recieved.txt in the program directory

	if (!recv_fd)
	{
		printf("Error IN Opening File ");
		return;
	}

	while (1)
	{
		memset(buff, 0x00, MAX);
		read_len = read(sockfd, buff, MAX);
		write(recv_fd, buff, read_len);
		if (read_len == 0)
		{
			printf("Download finish\n");
			break;
		}
	}
	// fprintf(fp,"%s",buff);

	printf("File received successfully !! \n");
	// printf("New File created is received.txt !! \n");
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		printf("setsockopt(SO_REUSEADDR) failed");

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for sending File
	recvFile(sockfd);

	// close the socket
	close(sockfd);
}