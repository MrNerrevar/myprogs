#include "unp.h"; //include appropriate header files for in built socket functions
#include <time.h>: //required to calculate date and time

int main(int argc, char **argv)
{
	int listenfd, connfd; // Socket ID's - One for listening socket and one for connected socket
	struct sockaddr_in servaddr; // Address structure to hold this servers address
	char buff(MAXLINE]; // buffer to hold sent data
	time_t ticks; // required to calculate date and time

	if (argc != 2) // verifies number of command line args.
		err_quit("usage: <Program Name> <Port No.>");

	listenfd = Socket(AF_INET, SOCK_STREAM, o); //Create socket
												//Different to client code which uses lower case 's'
												// error checking is wrapped in new Socket function.

	bzero(&servaddr, sizeof(servaddr)); // zero and fill in server address structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //connect to a local ip address
	servaddr.sin_port = htois(atoi(argv[1])); //daytime server port number

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr)); //connect to socket to an external interface

	Listen(listenfd, LISTENQ); //changes the socket to passive listening socket

	for( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL); //accept next connection request from client

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\r", ctime(&ticks)); //construct the data to return to the client
		Write(connfd, buff, strlen(buff)); //write data to the client

		Close(connfd);
	}
}

//Note the use of uppercase letters at the start of the socket primitive names. 
//These are wrapper functions that include the error checking functionality for each call. 
//The actual socket primitives use lower case