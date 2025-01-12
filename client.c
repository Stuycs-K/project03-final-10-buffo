#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "client.h"
#include "server.h"
#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server;
  from_server = client_handshake( &to_server );
  
  while(1){
    struct message m;
    struct message answer;
    printf("open, create, or close: ");
    fgets(m.command,sizeof(m.command),stdin);
    m.command[strlen(m.command) - 1] = '\0';
    printf("File name: ");
    fgets(m.text,sizeof(m.text),stdin);
    m.text[strlen(m.text) - 1] = '\0';
    write(to_server,&m,sizeof(m));
    int i = read(from_server,&answer,sizeof(answer));
    if(i <= 0){
      printf("server exited\n");
      break;
    }
    printf("%s\n", answer.command);
    printf("%s\n", answer.text);
  }
}

