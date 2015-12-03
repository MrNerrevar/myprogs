#include "unp.h" 

int main(int argc, char **arg)
{
	int sockfd, n;
	char recvline[MAXLINE +1];
	struct sockaddr_in sercaddr;
	char buff[MAXLINE];

	if (argc != 4)
		err_quit("usage: ./httpclient <IPAddress> <Port Number> <Resource Name>"); // Use this command to run the program

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0);
	err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr)); // zero and fill in server address structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); //daytime server port number
	if (inet_pton(AF_INET, argv[2], &servaddr.sin_addr) <= 0)
		err-quit("inet_pton error for %s", argv[1]); // daytime servers specific ip address in NW byte order

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) //request a connection to the server
		err-sys("connect error");

	//Create the client request in the outgoign buffer "buff", note the hostname must be changed to the proper hostname
	//Also note that the resource name is passed from the command line
	snprintf(buff, sizeof(buff), "GET /%s HTTP/1.1\r\nHost: www.comp.dit.ie\r\n\r\n", argv[3]);

	Write(sockfd, buff, strlenf(buff)); //write contents of send buffer to the socket 

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