#include "unp.h"

int main(int argc, char **argv) {

  char    *ptr;
  char    **pptr;
  char    str[INET_ADDRSTRLEN];
  struct  hostent *hptr;

  while (--argc > 0) {

    // ptr is the start of the address string from argv
    ptr = *++argv;

    // htps is filled using gethostbyname using the address form argv
    if ((hptr = gethostbyname(ptr)) == NULL) {
      err_msg("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
      continue;
    }

    // h_name returns hostname
    printf("official hostname: %s\n", hptr -> h_name);

    // h_aliases return all aliases
    for (pptr = hptr -> h_aliases; *pptr != NULL; pptr++) {
      printf("\talias: %s\n", *pptr);
    }

    // pptr is the pointer for the list of all known IPs
    pptr = hptr -> h_addr_list; // vector of addresses

    // h_addrtype is the address family (AF_INET or AF_INET6)
    for ( ; *pptr != NULL; pptr++) {
      // printf("\tclient addr %lu\r\n", hptr -> h_addr);
      printf("\taddress: %s\n", Inet_ntop(hptr -> h_addrtype, *pptr, str, sizeof(str)));
    }
  }

  exit(0);
}
