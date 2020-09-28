/*
 * End Goal: Measure Internet Statistics of sending 40 bytes
 * Analyze jitter/bunching on the network
 *
 * Compilation one-liner: g++ measureStatisticsServer.cpp -o measureStatisticsServer
 *
 * Code attribution-Basic server implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <string>

#define PORT 8080
#define MAXLINE 1024

const char* MESSAGE = "Hellohellohellohellohellohellohellohello"; //40 bytes

int main() {
  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;
  // Creating socket file descriptor 
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {       
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  } 
      
  memset(&servaddr, 0, sizeof(servaddr)); 
  memset(&cliaddr, 0, sizeof(cliaddr)); 
      
  // Filling server information 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(PORT);
  inet_aton("171.67.76.94", &servaddr.sin_addr);

  // Bind the socket with the server address 
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
  { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  } 
  while (true) { 
    unsigned int len = sizeof(cliaddr);  //len is value/result 
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
    buffer[n] = '\0'; 
    printf("Verify Client : %s\n", buffer); 
    sendto(sockfd, MESSAGE, strlen(MESSAGE),  
           MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
    printf("Hello message sent.\n"); 
  }
  return 0;
}
