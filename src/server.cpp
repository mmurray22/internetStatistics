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
#include <chrono>
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

// Creating socket file descriptor 
void create_socket_fd(int& sockfd) {
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  } 
}

//Filling server information
void complete_sockaddr_info(sockaddr_in& servaddr, sockaddr_in& cliaddr) {
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr)); 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;
}

int main() {
  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;
  unsigned int len = sizeof(cliaddr);
  create_socket_fd(sockfd); 
  complete_sockaddr_info(servaddr, cliaddr);

  // Bind the socket with the server address 
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  }
  auto start = std::chrono::steady_clock::now();
  while (true) { 
    auto start = std::chrono::steady_clock::now();
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
    buffer[n] = '\0'; 
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_lapse = end - start;
    if (time_lapse.count() > 0.0001) {
      printf("Packet processed in: %f\n", time_lapse.count());
    }
  }
  return 0;
}
