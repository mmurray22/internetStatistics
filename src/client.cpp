/*
 * Internet Statistics Client
 *
 * Compilation line: g++ client.cpp -o client
 *
 * Code attribution-Basic Client implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *
 *
 * Current Observations:
 * It says it takes 10s for it to send 4000 packets to the server
 * This is only 400 packets/sec, but we want 4000 packets/sec
 * It also seems that the packets are being sent after 2.5 +/- .00009
 * which seems quite good. So why the cut in packets/sec?
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
#define DELAY .00025

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
  while (true) {
    auto checkpoint = std::chrono::steady_clock::now();
    time_passed = (std::chrono::steady_clock::now()-start);
    //printf("Time passed: %f ms\n", time_passed.count()*1000);
    if (temp_reference + DELAY < time_passed.count()) {
      //printf("It took %f ms to send another packet!\n", (time_passed.count()-temp_reference)*1000);
      temp_reference = time_passed.count();
    } else {
      continue;
    }
    sendto(sockfd, (const char *)MESSAGE, strlen(MESSAGE), 
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
           sizeof(servaddr)); 
    number_of_packets++;
    if (number_of_packets > 4000) {
      break;
    }
  }
  auto end = std::chrono::steady_clock::now();
  time_passed = end-start;
  printf("Total time elapsed: %f s\n", time_passed.count());
  close(sockfd);
  return 0; 
} 
