#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>


#define BUF_LEN 1024

#define ERROR(msg) { perror((msg)); exit(errno); }


void usage() {
  fputs("Usage: getwebserver <server> [<port>]\n",stderr);
  exit(EXIT_FAILURE);
}


void request_header(int socket) {
  char *request = "GET /index.html HTTP/1.1\n\n";
  write(socket,request,strlen(request));
}

void print_header(int socket) {
  char buf[BUF_LEN];
  FILE *pf = popen("grep Server | head -n 1","w");
  int fd = fileno(pf);
  
  int n;
  do {
    n = read(socket,buf,BUF_LEN);
    write(fd, buf, n);
  }  while(n == BUF_LEN);
  write(fd, buf, n);
  write(fd,"\n", 1);
  fclose(pf);
}


int tcpconnection(char *host, const char *port) {
  struct addrinfo *result;
  
  int s =  getaddrinfo(host, port, NULL, &result);
  if( s ) {
    fprintf(stderr, "%s:%s: %s\n", host, port, gai_strerror(s));
    exit(errno);
  }

  struct sockaddr *server_addr = result->ai_addr;
  
  int sockfd = socket(server_addr->sa_family, SOCK_STREAM, 0);
  if(sockfd < 0) ERROR("socket");
  
  int size =  server_addr->sa_family==AF_INET ? sizeof(struct sockaddr_in) :
    sizeof( struct sockaddr_in6);
  
  if ( connect(sockfd, server_addr, size) <0 ) ERROR(host);

  freeaddrinfo(result);
  
  return sockfd;
}


int main(int args, char *argv[]){
  if(args < 2 || args > 3) usage();

  const char *port = (args == 3) ? argv[2] : "80";
  
  int sockfd = tcpconnection(argv[1],port);

  
  request_header(sockfd);
  print_header(sockfd);
  close(sockfd);

  return 0;
}
