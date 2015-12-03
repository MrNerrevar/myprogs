#include "unp.h"
#include <errno.h>
#include <string.h>

#define GOOGLE_SMTP_ADDRESS "64.233.166.26"
#define GOOGLE_SMTP_PORT 25

char* readLine(int);
char* commandBuilder(char*, char*, char*, char*, char*);
char input[MAXLINE];
char line[MAXLINE];
int sockfd;

int main(int argc, char **argv) {
  sockfd = 0;
  struct sockaddr_in servaddr;
  char mailfrom[MAXLINE];
  char mailto[MAXLINE];

  /*
  * Set up socket connection
  * open 64.233.166.26 25 <return key>
  */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    err_sys("socket error");
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(GOOGLE_SMTP_PORT);
  if (inet_pton(AF_INET, GOOGLE_SMTP_ADDRESS, &servaddr.sin_addr) <= 0) {
    err_quit("inet_pton error for %s", GOOGLE_SMTP_ADDRESS);
  }
  if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 ) {
    err_sys("connect error");
  }

  /*
  * Construct and send SMTP commands
  * (refer to commandBuilder method)
  *
  * HELO attu.ict.ad.dit.ie <return key>
  * MAIL FROM: <sender@email.com> <return key>
  * RCPT TO: <receiver@email.com> <return key>
  * DATA <return key>
  * From: sender@email.com <return key>
  * To: receiver@email.com <return key>
  * Subject: subject description <return key>
  * <return key>
  * Write body of the message here <return key>
  * <return key>
  * <full stop> <return key>
  */
  commandBuilder("220", "", "", "\0", "HELO 147.252.234.34\r\n");
  strcpy(mailfrom, commandBuilder("250", "\nMail from: ", "MAIL FROM: <", "\0", ">\r\n"));
  strcpy(mailto, commandBuilder("250", "\nMail to: ", "RCPT TO: <", "\0", ">\r\n"));
  commandBuilder("250", "", "", "\0", "DATA\r\n");
  commandBuilder("354", "", "From: ", mailfrom, "\r\n");
  commandBuilder("", "", "To: ", mailto, "\r\n");
  commandBuilder("", "\nSubject: ", "Subject: ", "\0", "\r\n");
  commandBuilder("", "", "", "\0", "\r\n");
  commandBuilder("", "\nBody: ", "", "\0", "\r\n");
  commandBuilder("", "", "", "\0", "\r\n");
  commandBuilder("", "", "", "\0", ".\r\n");

  if (strstr(readLine(sockfd), "250") != NULL) {
    printf("\nEmail sent succesfully.\n\n");
  } else {
    printf("\nProblem sending email. Check server response for info.\n\n");
  }
  exit(0);
}


/*
* Arguments
*   char *expected: expected server reply code after last command.
*   char *message: display input field description.
*   char *prefix: first part of command.
*   char *in: predefined field input.
*   char *postfix: ending of command.
* Return
*   char*: input string for optional storage.
*/
char* commandBuilder(char *expected, char *message, char *prefix, char *in, char *postfix) {
  char command[MAXLINE];
  if (strlen(expected) > 1) {
    char reply[MAXLINE];
    strcpy(reply, readLine(sockfd));
    if (strlen(reply) <= 0) {
      printf("No Server Response. Aborted.\n\n");
      exit(0);
    }
    if (strstr(reply, expected) == NULL) {
      printf("Bad server response. Something went wrong! Aborted.\n\n");
      exit(0);
    }
  }
  strcpy(input, in);
  strcpy(command, "\0");
  if (strlen(message) > 1) {
    printf("%s", message);
    while (fgets(input, MAXLINE, stdin) == NULL) {
      printf("Bad input. Enter again.\n %s", message);
    }
    if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n')) {
      input[strlen(input) - 1] = '\0';
    }
  }
  if (strlen(prefix) > 0) {
    strcat(command, prefix);
  }
  if (strlen(input) > 0) {
    strcat(command, input);
  }
  if (strlen(postfix) > 0) {
    strcat(command, postfix);
  }
  Write(sockfd, command, strlen(command));
  return input;
}


/*
* Read in server response
*/
char* readLine(int sock) {
  strcpy(line, "\0");
  int n = 0;
  int counter = 0;
  while((n = read(sock, line, MAXLINE)) > 0) {
    counter++;
    line[n] = 0; //null terminate
    if(fputs(line, stdout) == EOF) {
      err_sys("fputs error");
    }
    if(strstr(line, "\r\n\r\n") != NULL) {
      break;
    }
    break;
  }
  return line;
}
