#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "client.h"
#include <ctype.h>
#include "server.h"
#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server;
  from_server = client_handshake( &to_server );
  
  while(1){
    struct message m;
    struct message answer;
    printf("Open, Create, Close, Read, Modify, Clear, Exit: ");
    fgets(m.command,sizeof(m.command),stdin);
    m.command[strlen(m.command) - 1] = '\0';
    for(int i = 0; m.command[i]; i++){
      m.command[i] = tolower(m.command[i]);
    }
    if (strcmp(m.command, "open") != 0 && strcmp(m.command, "create") != 0 && strcmp(m.command, "close") != 0 && strcmp(m.command, "read") != 0 && strcmp(m.command, "modify") != 0 && strcmp(m.command, "clear") != 0 && strcmp(m.command, "exit") != 0){
      printf("Invalid command\n");
    }
    else{
      if(strcmp(m.command, "exit") != 0){
        printf("File name: ");
        fgets(m.filename,sizeof(m.filename),stdin);
        m.filename[strlen(m.filename) - 1] = '\0';
      }
      if(strcmp(m.command, "modify") == 0){
        printf("Text: ");
        fgets(m.text,sizeof(m.text),stdin);
      }
      write(to_server,&m,sizeof(m));
      if(strcmp(m.command, "exit") == 0){
        printf("Goodbye!\n");
        exit(0);
      }
      int i = read(from_server,&answer,sizeof(answer));
      if(i <= 0){
        printf("server exited\n");
        break;
      }
      printf("%s\n", answer.command);
      if(strcmp(answer.command, "File contents: ") == 0){
        printf("%s", answer.text);
      }
    }
  }
}

