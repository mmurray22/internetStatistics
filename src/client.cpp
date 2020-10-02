/*
 * Internet Statistics Client
 *
 * Compilation line: g++ client.cpp -o client
 *
 * Code attribution-Basic Client implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *
 *
 * Current Observations: Packet rate is ~4000 packets/sec!
 */
#include <arpa/inet.h> 
#include <chrono>
#include <cmath>
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <sys/types.h> 
#include <unistd.h> 

#define SERV_ADDR "171.67.76.94"
#define PORT 8080 
#define DELAY .0025

const char* MESSAGE = "Alohaalohaalohaalohaalohaalohaaloha"; //40 bytes

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

int main(int argc, char *argv[]) {
  int sockfd; 
  struct sockaddr_in servaddr;
  create_socket_fd(sockfd);
  complete_server_info(servaddr);
 
  auto start = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_passed; //Units: seconds
  double previous_time_passed = 0; //Units: seconds
  while (true) {
    time_passed = (std::chrono::steady_clock::now() - start);
    if (previous_time_passed + DELAY < time_passed.count()) {
      previous_time_passed = time_passed.count();
      sendto(sockfd, (const char *)MESSAGE, sizeof(MESSAGE), 
             MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
             sizeof(servaddr)); 
    }
  }
  close(sockfd);
  return 0; 
} 
