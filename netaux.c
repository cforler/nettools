#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netpacket/packet.h>

#include "netaux.h"

static char * mactostr(unsigned char sll[8], char *addr) {
  sprintf(addr, "%0x:%02x:%02x:%02x:%02x:%02x",
          sll[0], sll[1], sll[2], sll[3], sll[4], sll[5]);
  return addr;
}


char *addrtostr(struct sockaddr *sa) {
  if(!sa) return NULL;
  
  char *addr = malloc(NI_MAXHOST);
  memset(addr,0,NI_MAXHOST);

  const char *s;
  switch(sa->sa_family)  {
  case AF_INET: s = inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                              addr, NI_MAXHOST); break;
    
  case AF_INET6: s = inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                               addr, NI_MAXHOST); break;

  case AF_PACKET: s = mactostr(((struct sockaddr_ll *) sa)->sll_addr, addr);  break;

    
  default: errno=EFAULT; return NULL;
  }
  
  if(s) return addr;
  
  free(addr);
  return NULL;
}
