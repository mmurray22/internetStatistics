/*
 * Internet Statistics Client
 *
 * Compilation line: g++ measureStatisticsClient.cpp -o measureStatisticsClient
 *
 * Code attribution-Basic Client implementation found here:
 * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
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
  
#define PORT 8080 
#define MAXLINE 1024 

const char* MESSAGE = "Alohaalohaalohaalohaalohaalohaalohaaloha"; //40 bytes
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT);
    inet_aton("171.67.76.94", &servaddr.sin_addr);
      
    unsigned int len;
    double rttClient = 0;
    while (true) {  
      auto start = std::chrono::steady_clock::now();
      sendto(sockfd, (const char *)MESSAGE, strlen(MESSAGE), 
             MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
             sizeof(servaddr)); 
      auto end1 = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed_seconds1 = end1 - start;
      printf("Client Sent: %f ms\n", elapsed_seconds1.count()*1000); 
 
      int n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                       MSG_WAITALL, (struct sockaddr *) &servaddr, 
                       &len); 
      auto end2 = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed_seconds2 = end2 - start;
      buffer[n] = '\0'; 
      printf("Server Reply: %f ms\n", elapsed_seconds2.count()*1000); 
    }
    
    close(sockfd);
    return 0; 
} 
