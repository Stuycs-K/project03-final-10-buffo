#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "server.h"

struct line{
  int lineind;
  char linecontents[200];
  struct line *nextline;
};

struct document{
  int fileind;
  int w_file;
  struct document *nextdocument;
}
  

int main() {
  
  struct document *first;
  int to_client;
  int from_client;
  signal(SIGINT, signal_handler);
  signal(SIGPIPE, SIG_IGN);
  while(1){
    from_client = server_handshake( &to_client );
    while(1){
      int ran;
      int i = write(to_client,&ran,sizeof(ran));
      if(i <= 0){
        printf("client exited\n");
        break;
      }
    }
    close(from_client);
    close(to_client);
  }
}


int server_setup() {
  printf("server 1\n");
  mkfifo(WKP, 0644);
  printf("server 2\n");
  int p1 = open(WKP, O_RDONLY);
  unlink(WKP);
  printf("server 4\n");
  return p1;
}

int server_handshake(int *to_client) {
  int from_client = server_setup();
  char pp[100];
  read(from_client,pp,sizeof(pp));
  printf("server 5: %s\n",pp);
  printf("server 6\n");
  *to_client = open(pp, O_WRONLY);
  int synack = getpid();
  printf("server 7: %d\n",synack);
  write(*to_client,&synack,sizeof(synack));
  int finalack;
  printf("server 9\n");
  read(from_client,&finalack,sizeof(finalack));
  printf("server 9: %d\n",finalack);
  return from_client;
}

void server_handshake_half(int *to_client, int from_client) {
  char pp[100];
  read(from_client,pp,sizeof(pp));
  printf("server 5: %s\n",pp);
  printf("server 6\n");
  *to_client = open(pp, O_WRONLY);
  int synack = getpid();
  printf("server 7: %d\n",synack);
  write(*to_client,&synack,sizeof(synack));
  int finalack;
  printf("server 9\n");
  read(from_client,&finalack,sizeof(finalack));
  printf("server 9: %d\n",finalack);
}



int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
