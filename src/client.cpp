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
#define PORT 8080 
#define MAXLINE 1024 
#define DELAY .0025
#define START_TIME 0
#define PACKET_ZERO 0

const char* MESSAGE = "Alohaalohaalohaalohaalohaalohaaloha";
const int NUM_PACKET_MSG_ELTS = 5;
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

//TODO: This fxn will be used soon!
void initialize_packet_message(double* packet_msg) {
  memset(&packet_msg, 0, sizeof(packet_msg));
  packet_msg[0] = START_TIME;
  packet_msg[3] = PACKET_ZERO;
}

int main(int argc, char *argv[]) {
  int sockfd; 
  struct sockaddr_in servaddr;
  int number_of_packets = 0;
  double packet_msg[NUM_PACKET_MSG_ELTS];
  memset(&packet_msg, 0, sizeof(packet_msg));
  create_socket_fd(sockfd);
  complete_server_info(servaddr);
 
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
      sendto(sockfd, (const char *)MESSAGE, sizeof(MESSAGE), 
             MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
             sizeof(servaddr)); 
      number_of_packets++;
      //sanity_check(number_of_packets, start);
    }
  }
  close(sockfd);
  return 0; 
} 
