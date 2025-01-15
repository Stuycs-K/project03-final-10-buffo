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
  printf("File name: %s------\n", m.filename);
  
  struct file *temp = first;
  
  if (strcmp(m.command, "Open") == 0){
    while (temp != NULL && strcmp(temp->name, m.filename) != 0){
      temp = temp->nextfile;
    }
    if(temp != NULL){
      strcpy(answer->command, "File already open");
      strcpy(answer->filename, m.filename);
    }
    else{
      int fd = open(m.filename, O_RDWR, 0611);
      if(fd == -1){
        strcpy(answer->command, "File does not exist");
        strcpy(answer->filename, m.filename);
      }
      else{
        struct file* new = (struct file*) malloc(sizeof(struct file));
        printf("Open\n");
        new->w_file = fd;
        new->nextfile = first;
        strcpy(new->name, m.filename);
        first = new;
        strcpy(answer->command, "File opened");
        strcpy(answer->filename, m.filename);
      }
    }
  }
  else if (strcmp(m.command, "Create") == 0){
    while (temp != NULL && strcmp(temp->name, m.filename) != 0){
      temp = temp->nextfile;
    }
    if(temp != NULL){
      strcpy(answer->command, "File already created");
      strcpy(answer->filename, m.filename);
    }
    else{
      int fd = open(m.filename, O_RDWR, 0611);
      if(fd != -1){
        strcpy(answer->command, "File already exists");
        strcpy(answer->filename, m.filename);
        close(fd);
      }
      else{
        int fd = open(m.filename, O_CREAT | O_TRUNC | O_RDWR, 0611);
        if(fd == -1){
          strcpy(answer->command, "File cannot be created");
          strcpy(answer->filename, m.filename);
        }
        else{
          struct file* new = (struct file*) malloc(sizeof(struct file));
          printf("Open\n");
          new->w_file = fd;
          new->nextfile = first;
          strcpy(new->name, m.filename);
          first = new;
          strcpy(answer->command, "File created");
          strcpy(answer->filename, m.filename);
        }
      }
    }
  }
  else if (strcmp(m.command, "Close") == 0){
    struct file* temp1 = NULL;
    while (temp != NULL && strcmp(temp->name, m.filename) != 0){
      temp1 = temp;
      temp = temp->nextfile;
    }
    if(temp == NULL){
      strcpy(answer->command, "File not open");
      strcpy(answer->filename, m.filename);
    }
    else{
      close(temp->w_file);
      if(temp1 == NULL){
        first = temp->nextfile;
      }
      else{
        temp1->nextfile = temp->nextfile;
      }
      free(temp);
      strcpy(answer->command, "File closed");
      strcpy(answer->filename, m.filename);
    }
  }
  else if (strcmp(m.command, "Read") == 0){
    while (temp != NULL && strcmp(temp->name, m.filename) != 0){
      temp = temp->nextfile;
    }
    if(temp == NULL){
      strcpy(answer->command, "File not open");
      strcpy(answer->filename, m.filename);
    }
    else if (temp != NULL){
      lseek(temp->w_file, 0, SEEK_SET);
      int n = read(temp->w_file, answer->text, sizeof(answer->text));
      answer->text[n] = '\0';
      if (strcmp(answer->text, "") == 0){
        strcpy(answer->command, "File has no contents");
      }
      else{
        strcpy(answer->command, "File contents: ");
      }
    }
  }
  else if (strcmp(m.command, "Modify") == 0){
    while (temp != NULL && strcmp(temp->name, m.filename) != 0){
      temp = temp->nextfile;
    }
    if(temp == NULL){
      strcpy(answer->command, "file not open");
      strcpy(answer->filename, m.filename);
    }
    else{
      lseek(temp->w_file, 0, SEEK_END);
      write(temp->w_file, m.text, strlen(m.text));
      strcpy(answer->command, "File modified");
    }
  }
  else if (strcmp(m.command, "Clear") == 0){
    while (temp != NULL && strcmp(temp->name, m.filename) != 0){
      temp = temp->nextfile;
    }
    if(temp == NULL){
      strcpy(answer->command, "file not open");
      strcpy(answer->filename, m.filename);
    }
    else{
      fopen(m.filename, "w");
      strcpy(answer->command, "File cleared");
    }
  }
  else{
    printf("Invalid command: %s\n", m.command);
    strcpy(answer->command, "Invalid command");
    strcpy(answer->filename, m.filename);
  }
}
