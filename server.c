/*
   Isaiah Kovacich
   */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  
#include <stdlib.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>

#define PORT 20001
#define BUFFERSIZE 1024

bool running = true;
int socket_fd;

void handleCon(){
  printf("handleCon\n");
  char buffer[BUFFERSIZE] = {0}; 
  while(running){
    long ret = read(socket_fd, buffer, BUFFERSIZE);
    printf("%d\n", ret); 
    if(ret == 0 || ret == -1) {
        printf("ret bad size\n");
        running=false;
        continue;
    }
    /*if(ret > 0 && ret < BUFFERSIZE)  
        buffer[ret]=0;  
    else buffer[0]=0;*/
    for(int i=0; i<ret;i++){
      printf("%c",*(buffer+i) ); 
    }
    const char* hello = "helllllljeujs\n";
    send(socket_fd , hello , strlen(hello) , 0 ); 
    printf("loophandleCon\n");
  }
  close(socket_fd);
  printf("exithandleCon\n");
}

int main(){
	printf("Started loop\n");
  int server_fd; 
  struct sockaddr_in address; 
  int opt = 1; 
  int addrlen = sizeof(address); 
  if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0) { 
      perror("socket failed"); 
      exit(EXIT_FAILURE); 
  } 
  address.sin_family = AF_INET; 
  address.sin_addr.s_addr = INADDR_ANY; 
  address.sin_port = htons( PORT ); 
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){ 
      perror("bind failed"); 
      exit(EXIT_FAILURE); 
  }
  if (listen(server_fd, 3) < 0){
        perror("listen"); 
        exit(EXIT_FAILURE); 
  }
  while(running){
    if ((socket_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        perror("accept");
        continue;
    }else{
      pid_t processID = fork();
      if (processID < 0){
        perror("fork");
      }else if (processID == 0){
        close(server_fd);
        handleCon();
        exit(0);
      }
      socket_fd=-1;
    }
  }
  close(server_fd);
	printf("Shutting down\n");
	return 0;
}