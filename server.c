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

int main() {
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
        write(to_client,&answer,sizeof(answer));
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
  
  struct file *temp = first;
  
  if (strcmp(m.command, "open") == 0){
    while (temp != NULL && strcmp(temp->name, m.text) != 0){
      temp = temp->nextfile;
    }
    if(temp != NULL){
      strcpy(answer->command, "already open");
      strcpy(answer->text, m.text);
    }
    else{
      int fd = open(m.text, O_RDWR, 0611);
      if(fd == -1){
        strcpy(answer->command, "file does not exist");
        strcpy(answer->text, m.text);
      }
      else{
        struct file* new = (struct file*) malloc(sizeof(struct file));
        printf("open\n");
        new->w_file = fd;
        new->nextfile = first;
        strcpy(new->name, m.text);
        first = new;
        strcpy(answer->command, "opened");
        strcpy(answer->text, m.text);
      }
    }
  }
  else if (strcmp(m.command, "create") == 0){
    while (temp != NULL && strcmp(temp->name, m.text) != 0){
      temp = temp->nextfile;
    }
    if(temp != NULL){
      strcpy(answer->command, "already created");
      strcpy(answer->text, m.text);
    }
    else{
      int fd = open(m.text, O_RDWR, 0611);
      if(fd != -1){
        strcpy(answer->command, "file already exists");
        strcpy(answer->text, m.text);
        close(fd);
      }
      else{
        int fd = open(m.text, O_CREAT | O_TRUNC | O_RDWR, 0611);
        if(fd == -1){
          strcpy(answer->command, "file cannot be created");
          strcpy(answer->text, m.text);
        }
        else{
          struct file* new = (struct file*) malloc(sizeof(struct file));
          printf("open\n");
          new->w_file = fd;
          new->nextfile = first;
          strcpy(new->name, m.text);
          first = new;
          strcpy(answer->command, "created");
          strcpy(answer->text, m.text);
        }
      }
    }
  }
  else if (strcmp(m.command, "close") == 0){
    while (temp != NULL && strcmp(temp->name, m.text) != 0){
      temp = temp->nextfile;
    }
    if(temp == NULL){
      strcpy(answer->command, "file not open");
      strcpy(answer->text, m.text);
    }
    else{
      close(temp->w_file);
      free(temp);
      strcpy(answer->command, "closed");
      strcpy(answer->text, m.text);
    }
  }
  else{
    printf("Invalid command: %s\n", m.command);
  }
}
