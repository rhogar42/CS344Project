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

int main() 
{ 
    struct sockaddr_in address; 
    int sock = 0; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){ 
        perror("Socket creation error"); 
        return -1; 
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
        perror("Invalid address, Address not supported"); 
        return -1; 
    } 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        perror("Connection Failed"); 
        return -1; 
    } 
    
    char * str = (char*)malloc(BUFFERSIZE);
    char buffer[BUFFERSIZE] = {0};
    while(running){
      fgets(str, BUFFERSIZE, stdin);
      fflush(stdin); //doesn't do anything, even when overflowed.
      send(sock , str , strlen(str) , 0 );
      long ret = read(sock, buffer, BUFFERSIZE);
      printf("%d\n", ret); 
      if(ret == 0 || ret == -1) {
        printf("ret bad size\n");
        running=false;
        continue;
      }
      for(int i=0; i<ret;i++){
        printf("%c",*(buffer+i)); 
      }
    }
    close(sock);
    return 0; 
} 