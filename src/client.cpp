/*
 * Internet Statistics Client
 *
 * Compilation line: g++ client.cpp -o client
 *
 * Code attribution-Basic Client implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 */
#include <chrono>
#include <cmath>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define SERV_ADDR "171.67.76.94"
#define PORT 7654 
#define MAXLINE 1024 
#define DELAY .0025

const char* MESSAGE = "Alohaalohaalohaalohaalohaalohaalohaaloha"; //40 bytes

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
  
  int number_of_packets = 1;
  auto start = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_passed;
  double temp_reference = 0;

  sendto(sockfd, (const char *)MESSAGE, strlen(MESSAGE), 
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
           sizeof(servaddr)); 
  while (true) {
    auto checkpoint = std::chrono::steady_clock::now();
    time_passed = (std::chrono::steady_clock::now()-start);
    if (temp_reference + DELAY > time_passed.count()) {
      printf("It took %f ms to send another packet!", temp_reference*1000);
      temp_reference = time_passed.count();
      continue;
    }
    sendto(sockfd, (const char *)MESSAGE, strlen(MESSAGE), 
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
           sizeof(servaddr)); 
    number_of_packets++;
    if (number_of_packets > 10000) {
      break;
    }
  }
  close(sockfd);
  return 0; 
} 
