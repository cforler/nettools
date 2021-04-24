#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "netaux.h"

void usage() {
  fputs("Usage: ./getip [-a] <host>\n",stderr);
  exit(EXIT_FAILURE);
}

void init_addrinfo(struct addrinfo *p) {
  memset(p, 0, sizeof(struct addrinfo));
  p->ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  p->ai_socktype = 0;
  p->ai_flags = AI_PASSIVE;    /* For wildcard IP address */
  p->ai_protocol = 0;          /* Any protocol */
  p->ai_canonname = NULL;
  p->ai_addr = NULL;
  p->ai_next = NULL;
}



int main(int args, char *argv[]){
  if(args < 2 || args > 3)  usage();
  
  struct addrinfo *result;
  struct addrinfo hints;
  init_addrinfo(&hints);

  int flags = 0;
  if(args == 3) {
    if( (strlen(argv[1]) == 2) && strncmp(argv[1],"-a",2)==0) 
      flags=1;
      else usage();
  }
    
   
  int s =  getaddrinfo(argv[args-1], NULL, NULL, &result);
  if( s ) {
    fprintf(stderr, "%s: %s\n", argv[args-1], gai_strerror(s));
    exit(errno);
  }

  FILE *pf = popen("uniq","w");
  if(!pf) ERROR("uniq");
  
  for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
    char *ip = addrtostr(rp->ai_addr);
    if(!ip) ERROR("inet_ntop");
    fprintf(pf,"%s\n",ip);  
    free(ip);
    if(!flags) break;
  }
  pclose(pf);
  freeaddrinfo(result);
}
