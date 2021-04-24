 #define _GNU_SOURCE  
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/if_link.h>

#include "netaux.h"
#include "queue.h"

struct netface {
  struct netface  *nef_next; 
  char *nef_name;
  queue_t *nef_q;
};

void freenetface(struct netface nef) {
  struct netface *last = &nef;
  for(struct netface *n = &nef; n != NULL; n = n->nef_next) {
    if(last != &nef) free(last); 
    queue_free(n->nef_q);
    last = n;
  }
  if(last != &nef) free(last); 
}
  

void iface_add(struct netface *nef, struct ifaddrs *ifa) {
  struct netface *last = nef;
  
  for(struct netface *n = nef; n != NULL; n = n->nef_next) {
    if( strcmp(n->nef_name, ifa->ifa_name) == 0 ) {
      queue_add(n->nef_q, ifa);
      return;
    }
    last = n;
  }
  struct netface *nf = malloc(sizeof(struct netface));
  nf->nef_next = NULL;
  nf->nef_name = ifa->ifa_name;  
  nf->nef_q = queue_new();
  queue_add(nf->nef_q, ifa);
  
  last->nef_next = nf;
}


void print_netface(struct netface *nef) {
  for(struct netface *n = nef; n != NULL; n = n->nef_next) {
    printf("%s: \n", n->nef_name);
    for(struct ifaddrs *ifa = queue_remove(n->nef_q);
        ifa != NULL; ifa = queue_remove(n->nef_q)) {
      
      switch(ifa->ifa_addr->sa_family) {
      case AF_INET:
        printf("\tinet %s   netmask %s   broadcast %s\n",
               addrtostr(ifa->ifa_addr), 
               addrtostr(ifa->ifa_netmask),
               addrtostr(ifa->ifa_ifu.ifu_broadaddr));
        break;
        
      case AF_INET6:
        printf("\tinet6 %s   netmask %s\n",
               addrtostr(ifa->ifa_addr), 
               addrtostr(ifa->ifa_netmask));
        break;
      case AF_PACKET:
        printf("\tether %s\n", addrtostr(ifa->ifa_addr));
        
        struct rtnl_link_stats *stats = ifa->ifa_data;
        printf("\ttx_packets = %8u   rx_packets = %8u\n"
               "\ttx_bytes   = %8u   rx_bytes   = %8u\n",
               stats->tx_packets, stats->rx_packets,
               stats->tx_bytes, stats->rx_bytes);
         
      } // switch
    } // for (queue)
    if(n->nef_next)  puts("");
  } //for (netface)
}



int main() {
  struct ifaddrs *ifaddr;
   
  if (getifaddrs(&ifaddr) == -1) ERROR("getifaddrs");

  struct netface nef;
  nef.nef_next = NULL;
  nef.nef_q  = queue_new();
  nef.nef_name = ifaddr->ifa_name;
  
  
  for (struct ifaddrs *ifa = ifaddr; ifa != NULL;
       ifa = ifa->ifa_next) {
    
    if (ifa->ifa_addr == NULL) continue;
    iface_add(&nef, ifa);
  }    
  print_netface(&nef);
  freeifaddrs(ifaddr);
  freenetface(nef);

  exit(EXIT_SUCCESS);
}
