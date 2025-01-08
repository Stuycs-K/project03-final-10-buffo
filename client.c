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
    printf("Open or create: ");
    fgets(m.command,sizeof(m.command),stdin);
    printf("File name: ");
    fgets(m.text,sizeof(m.text),stdin);
    write(to_server,&m,sizeof(m));
  }
}

