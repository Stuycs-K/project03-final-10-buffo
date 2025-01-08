#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include "server.h"
#include "pipe_networking.h"

struct document *first = NULL;

void signal_handler(int signum) {
  printf("server exited\n");
  unlink(WKP);
  while(first != NULL){
    struct document *p = first;
    first = first->nextdocument;
    close(first->w_file);
    free(first);
  }
  exit(1);
}

int main() {
  int to_client;
  int from_client;
  signal(SIGINT, signal_handler);
  signal(SIGPIPE, SIG_IGN);
  while(1){
    int from_client = server_setup();
    pid_t p = fork();
    if(p == 0){
      server_handshake_half( &to_client, from_client );
      while(1){
        int ran = 10;
        int i = write(to_client,&ran,sizeof(ran));
        if(i <= 0){
          printf("client exited\n");
          break;
        }
        sleep(1);
      }
      close(from_client);
      close(to_client);
      exit(0);
    }
    sleep(1);
  }
}
