#include "unp.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	socklen_t len; //this var is used for obtaining the client addressing information. See accept cell
	int n, listenfd, connfd, char_in, count = 0; //declare usual socket variables and extra vars for reading from file
	struct sockaddr_in servaddr, cliaddr; //declare address structures for both client and server addressing data
	char buff[40], wbuff[MAXLINE], rbuff[MAXLINE], cmd[16], path1[64]=".", path[64], vers[16]; //declare read and write buffers
														//extra buffers for parsing client request and printing silent address

	FILE * hfile; // declare file pointer

	if (argc != 2)
		err_quit("usage: a.out <Port>");

	listenfd = Socket(AD_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr)); // zero and fill in server address structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //connect to a local ip address
	servaddr.sin_port = htois(atoi(argv[1])); //daytime server port number

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr)); //connect to socket to an external interface

	Listen(listenfd, LISTENQ); //changes the socket to passive listening socket

	for ( ; ; ) //infinite loop for dealing with client connections
	{
		len = sizeof(cliaddr); // set len to sizeof cliaddr struct - to be used to obtain client addressing data

		connfd = Accept(listenfd, (SA *) &cliaddr, &len); //accept the next client connection request and retrieve client address

		// printing the client IP address and port number. 
		// Note the use of ntop and ntohs functions for conversion from network byte order to host byte order
		printf("\nConnection from %s, port %d\n", Inet_ntop,(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

		while ((n = read(connfd, rbuff, MAXLINE)) > 0) //read loop
		{
			rbuff[n] = 0; //null terminate rbuff prior to screen print

			if (fputs(rbuff, stdout) == EOF) //screen print contents of read buffer
				err_sys("fputs error");

			//Looks for a needle in the haystack char *strstr(const char *haystack, const char *needle);
			//It returns a pointer to the location within the haystack.
			//If not found it returns to zero.
			//Used to break from read loop
			if (strstr(rbuff, "\r\n\r\n") > 0)
			{
				break;
			}
		}

		if (n < 0) //error check on read loop
			err_sys("read error");

		sscanf(rbuff, "%s %s %s", cmd, path, vers); //parsing the incoming client request

		strcat(path1, path); //concatenate the resource name to a full stop to refer to "this" directory

		if (strcmp(path1, "./") == 0)
		{
			strcopy(path1, "./index.html"); //check for empty "slash" and replace with default page
		}

		hFile = fopen(path1, "r"); //open the requested file

		if (hFile == NULL) //if requuested file does not exist
		{
			hFile = fopen("error.html"); //open error file
		}

		strcopy(wbuff,""); //empty the outgoing buffer

		while((char_in = fgetc(hFile)) != EOF) //reading one char at a time from the open file
		{
			wbuff[count] = char_in; //storing the char in the outgoing buffer
			count++; //increment the buffer index ready for next character
		}

		wbuff[count] = 0; //null terminate outgoing buffer readuy for writing to socket

		Write(connfd, wbuff, strlen(wbuff)); //write contents of buff to the socket
		count = 0; //reset buffer index
		fclose(hFile); //close the file
		strcopy(path1, "."); //reset path buffer back to "."

		Close(connfd); //close the connection
	}
}