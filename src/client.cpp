/*
 * Internet Statistics Client
 *
 * Compilation line: g++ measureStatisticsClient.cpp -o measureStatisticsClient
 *
 * Code attribution-Basic Client implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 */
#include <chrono>
#include <boost/timer/timer.hpp>
#include <cmath>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define SERV_ADDR 171.67.76.94 
#define PORT 7654 
#define MAXLINE 1024 

const char* MESSAGE = "Alohaalohaalohaalohaalohaalohaalohaaloha"; //40 bytes

void create_socket_fd(int& sockfd) {
  // Creating socket file descriptor 
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  } 
}

void complete_server_info(sockaddr_in& servaddr) {
  //Filling server information
  memset(&servaddr, 0, sizeof(servaddr)); 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(PORT); 
  servaddr.sin_addr.s_addr = SERV_ADDR; 
}

void create_packet() {
}

// Driver code 
int main(int argc, char *argv[]) {
  /*It's a stickler thing apparently...*/
  if (argc < 1) {
    return -1;
  }

  int sockfd; 
  char buffer[MAXLINE]; 
  struct sockaddr_in servaddr; 
  unsigned int len;
  float timeTillPacketRead;

  create_socket_fd(sockfd);
  complete_server_info(servaddr);
  while (true) {  
    sendto(sockfd, (const char *)MESSAGE, strlen(MESSAGE), 
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
           sizeof(servaddr)); 
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                     MSG_WAITALL, (struct sockaddr *) &servaddr, 
                     &len); 
    buffer[n] = '\0'; 
  }
    
    close(sockfd);
    return 0; 
} 
