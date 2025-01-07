#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "client.h"

int main() {
  int to_server;
  int to_client;
  to_client = client_handshake( &to_server );
  
  while(1){
    
    
    int ran;
    int i = read(to_client,&ran,sizeof(ran));
    if(i <= 0){
      exit(1);
    }
    printf("%d\n", ran);
  }
  
  while(1){
    printf("\nRead or create file? R/C: ");
    char ans1;
    fgets(ans,255,stdin);
    if (strcmp(ans1, 'R') == 0){
      int w_file = open(filename, O_WRONLY, 0611);
      printf("\nEdit line Y/N: ");
      char ans;
      fgets(ans,255,stdin);
      if (strcmp(ans, 'Y') == 0){
        printf("\nEnter line as wished: ");
        char lin[200];
        fgets(lin,255,stdin);
        *to_server = open(WKP, O_WRONLY);
        write(*to_server,lin,sizeof(lin));
      }
    }
  }
}

int client_handshake(int *to_server) {
  
  char pp[100];
  sprintf(pp,"%d",getpid());
  printf("client 3: %s\n",pp);
  mkfifo(pp, 0644);
  *to_server = open(WKP, O_WRONLY);
  write(*to_server,pp,sizeof(pp));
  int downstream = open(pp, O_RDONLY);
  printf("client 8\n");
  unlink(pp);
  int synack;
  read(downstream,&synack,sizeof(synack));
  printf("client 8: %d\n",synack);
  synack++;
  printf("client 8: %d\n",synack);
  write(*to_server,&synack,sizeof(synack));
  return downstream;
}
