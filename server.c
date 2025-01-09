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

int do_command(struct message m){
  printf("%s\n", m.command);
  printf("%s\n", m.text);

  if (strcmp(m.command, “open");
  {
    struct file *temp = firstfile;
    while (temp != NULL && strcmp(temp.name, m.text) != 0){
      temp = temp.nextfile;
    }
    if(temp == NULL){
       // create file
    }
    else{
      printf(“open\n”);
      temp.w_file = open(temp.name, O_RDONLY, 0611);
    }
}

int main() {
  int from_client;
  signal(SIGINT, signal_handler);
  signal(SIGPIPE, SIG_IGN);
  while(1){
    int from_client = server_setup();
    pid_t p = fork();
    if(p == 0){
      int to_client = server_handshake_half( from_client );
      while(1){
        struct message m;
        int i = read(from_client,&m,sizeof(m));
        if(i <= 0){
          printf("client exited\n");
          break;
        }
        do_command(m);
      }
      close(from_client);
      close(to_client);
      exit(0);
    }
    sleep(1);
  }
}
