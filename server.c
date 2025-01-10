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

int main() {
  //struct file *first = NULL;
  int from_client;
  //signal(SIGINT, signal_handler);
  //signal(SIGPIPE, SIG_IGN);
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

/*void signal_handler(int signum, struct file *first) {
  printf("server exited\n");
  unlink(WKP);
  while(first != NULL){
    struct file *p = first;
    first = first->nextfile;
    close(first->w_file);
    free(first);
  }
  exit(1);
}*/

void do_command(struct message m, struct message *answer){
  
  printf("Command: %s------\n", m.command);
  printf("File name: %s------\n", m.text);
  
  struct file *temp = NULL;
  
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
    printf("1\n");
    while (temp != NULL){
      printf("2\n");
      temp = temp->nextfile;
    }
    while (temp != NULL && strcmp(temp->name, m.text) != 0){
      printf("3\n");
      temp = temp->nextfile;
    }
    if(temp != NULL){
      printf("4\n");
      strcpy(answer->command, "failed");
      strcpy(answer->text, temp->name);
    }
    else{
      printf("5\n");
      //strcpy(temp->name, m.text);
      printf("create\n");
      printf("test\n");
      temp->w_file = open(m.text, O_CREAT, 0611);
      printf("file created\n");
      strcpy(answer->command, "success\n");
      strcpy(answer->text, temp->name);
    }
  }
  else{
    printf("Invalid command: %s\n", m.command);
  }
}
