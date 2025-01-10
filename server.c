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

struct file *first = NULL;

void signal_handler(int signum) {
  printf("server exited\n");
  unlink(WKP);
  while(first != NULL){
    struct file *p = first;
    first = first->nextfile;
    close(first->w_file);
    free(first);
  }
  exit(1);
}

void do_command(struct message m, struct message *answer){
  
  struct file *temp = first;
  
  if (strcmp(m.command, "open") == 0){
    while (temp != NULL && strcmp(temp->name, m.text) != 0){
      temp = temp->nextfile;
    }
    if(temp == NULL){
      strcpy(answer->command, "failed");
      strcpy(answer->text, temp->name);
    }
    else{
      printf("open\n");
      temp->w_file = open(temp->name, O_RDONLY, 0611);
      strcpy(answer->command, "success");
      strcpy(answer->text, temp->name);
    }
  }
  else if (strcmp(m.command, "create") == 0){
    while (temp != NULL){
      temp = temp->nextfile;
    }
    while (temp != NULL && strcmp(temp->name, m.text) != 0){
      temp = temp->nextfile;
    }
    if(temp != NULL){
      strcpy(answer->command, "failed");
      strcpy(answer->text, temp->name);
    }
    else{
      printf("create\n");
      temp->w_file = open(temp->name, O_CREAT, 0611);
      strcpy(answer->command, "success");
      strcpy(answer->text, temp->name);
    }
  }
  else{
    printf("Invalid command: %s\n", m.command);
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
        struct message answer;
        int i = read(from_client,&m,sizeof(m));
        if(i <= 0){
          printf("client exited\n");
          break;
        }
        do_command(m, &answer);
      }
      close(from_client);
      close(to_client);
      exit(0);
    }
    sleep(1);
  }
}
