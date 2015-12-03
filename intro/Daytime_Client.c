#include "unp.h" //include appropriate header file for in built socket functionality

int main(int argc, char **argv)
{
	int sockfd, n; //Socket ID
	char recvline[MAXLINE + 1]; //Buffer to hold read data
	struct sockaddr_in servaddr; //Address structure to hold the server address

	if (argc != 3) //verifies number of command line args.
		err_quit("usage: <Program Name> <IPAddress> <Port No.>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //create socket
		err_sys("Socket error");

	bzero(&servaddr, sizeof(servaddr)); // zero and fill in server address structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); //daytime server port number
	if (inet_pton(AF_INET, argv[2], &servaddr.sin_addr) <= 0)
		err-quit("inet_pton error for %s", argv[1]); // daytime servers specific ip address in NW byte order

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) //request a connection to the server
		err-sys("connect error");

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) // Read data from the server
	{
		recvline[n] = 0; // null termionate data from server
		if(fputs(recvline, stdout) == EOF) // print to the screen
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error");

	exit(0); // exit the program implies socket is closed
}