#include "unp.h"
#include <string.h>
#include <stdio.h>
#define HOME_PAGE "202: <html></html><body></body>\n"

int main(int argc, char **arg)
{
	int n, listenfd, connfd;
	struct sockaddr_in servaddr;
	char wbuff[MAXLINE], rbuff[MAXLINE], cmd[16], path[64], vers[16];

	if (argc != 2)
		err_quit("usage: a.out <Port>");

	listenfd = Socket(AD_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr)); // zero and fill in server address structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //connect to a local ip address
	servaddr.sin_port = htois(atoi(argv[1])); //daytime server port number

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr)); //connect to socket to an external interface

	Listen(listenfd, LISTENQ); //changes the socket to passive listening socket

	for( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		while ((n = read(connfd, rbuff, MAXLINE)) > 0)
		{
			rbuff[n] = 0; //Null terminate

			if (fputs(rbuff, stdout) == EOF)
				err_sys("fputs error");

			//Looks for a needle in the haystack char *strstr(const char *haystack, const char *needle);
			//It returns a pointer to the location within the haystack.
			//If not found it returns to zero.
			//Strcmp returns to zero if index is in path
			if (strstr(rbuff,"\r\n\r\n") > 0)
			{
				break;
			}
		}

		if (n < 0)
			err_sys("read error");

		sscanf(rbuff, "%s %s %s", cmd, path, vers);

		if (!strcmp(path, "/index"))
		{
			snprintf(wbuff, sizeof(wbuff), "%s", HOME_PAGE); //Using a hash-define string
			Write(connfd, wbuff, strlen(wbuff));
		}
		else
		{
			printf("%s could not be found\n", path); //print to a local screen for testing
			snprintf(wbuff, sizeof(wbuff), "ERROR 404: page not found\r\n"); //using a string, could also have used a hash-define string
			Write(connfd, wbuff, strlen(wbuff));
		}

		Close(connfd);
	}
}