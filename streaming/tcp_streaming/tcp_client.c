#include "tcp.h"

void recvFile(int sockfd, const char *filename)
{
	char buff[MAX]; // to store message from client
	int recv_fd, read_len, file_name_len;
	int ret = 0;
	int exist;

	// strcpy(filename_buff, filename);
	file_name_len = strlen(filename) + 1;

	printf("sending filename > %s with size > %d\n", filename, file_name_len);

	ret = send(sockfd, filename, file_name_len, 0);
	if (ret < 0)
	{
		printf("Failed to send filename\n");
		return;
	}

	ret = read(sockfd, &exist, sizeof(exist));
	if (ret < 0)
	{
		printf("Failed to receive data about file existence\n");
	}

	//printf("Value of exits in client > %d\n", ntohl(exist));

	if (exist < 0)
	{
		printf("Failed: File doesn't exist at the server\n");
		return;
	}

	recv_fd = open("recieved1.mp4", O_WRONLY | O_CREAT | O_TRUNC,
				   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (!recv_fd)
	{
		printf("Error IN Opening File ");
		return;
	}

	while (1)
	{
		memset(buff, 0x00, MAX);
		read_len = read(sockfd, buff, MAX);
		ret = write(recv_fd, buff, read_len);
		if (ret < 0)
		{
			printf("Failed to recieve/write data to the server\n");
		}
		if (read_len == 0)
		{
			printf("Download finish\n");
			printf("File received successfully!!\n");
			break;
		}
	}
}

int run_tcp_client(const char *filename)
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	printf("file requested by client: %s\n", filename);

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
	recvFile(sockfd, filename);

	// close the socket
	close(sockfd);
}