/*
 * End Goal: Measure Internet Statistics of sending 40 bytes
 * Analyze jitter/bunching on the network
 *
 * Compilation one-liner: g++ server.cpp -o server
 *
 * Code attribution-Basic server implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *
 * Current Observations:
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

#define SERV_ADDR "171.67.76.94"
#define PORT 8080
#define MAXLINE 1024

// Creating socket file descriptor 
void create_socket_fd(int& sockfd) {
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  } 
}

//Filling server information
void complete_server_info(sockaddr_in& servaddr) {
  memset(&servaddr, 0, sizeof(servaddr)); 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(PORT);
  inet_aton(SERV_ADDR, &servaddr.sin_addr);
}

/*void complete_client_info(sockaddr_in& cliaddr) {
  memset(&cliaddr, 0, sizeof(cliaddr)); 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(PORT);
}*/

int main() {
  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;
  create_socket_fd(sockfd); 
  complete_server_info(servaddr);
  //complete_client_info(cliaddr);

  // Bind the socket with the server address 
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  }

  while (true) { 
    unsigned int len = sizeof(cliaddr);  //len is value/result 
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
    buffer[n] = '\0'; 
    printf("Verify Client : %s\n", buffer); 
  }
  return 0;
}
