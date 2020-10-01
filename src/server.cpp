/*
 * End Goal: Measure Internet Statistics of sending 40 bytes
 * Analyze jitter/bunching on the network
 *
 * Compilation one-liner: g++ server.cpp -o server
 *
 * Code attribution
 * -Basic server implementation found here: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 * -Help with server timing issue here: https://stackoverflow.com/questions/16165542/is-it-possible-to-read-the-ttl-ip-header-field-when-receiving-udp-packets
 *
 * Current Observations:
 */

#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/ioctl.h>
#include <inttypes.h>
#include <linux/errqueue.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <string>
#include <vector>

#define PORT 8080
#define MAXLINE 40

// Creating socket file descriptor 
void create_socket_fd(int& sockfd, int& timestampOn) {
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
    perror("socket creation failed"); 
    exit(EXIT_FAILURE); 
  }
  if (setsockopt(sockfd, SOL_SOCKET, SO_TIMESTAMPNS, &timestampOn, sizeof(timestampOn)) < 0) {
    perror("unable to request socket timestamps");
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

void complete_message_info(struct msghdr& mhdr, struct iovec* iov, sockaddr_in& servaddr, char* buffer, char* ctrlDataBuf) {
  mhdr.msg_name = &servaddr;
  mhdr.msg_namelen = sizeof(servaddr);
  mhdr.msg_iov = iov;
  mhdr.msg_iovlen = 1;
  mhdr.msg_control = &ctrlDataBuf;
  mhdr.msg_controllen = sizeof(ctrlDataBuf);
  iov[0].iov_base = buffer;
  iov[0].iov_len = sizeof(buffer);
  memset(buffer, 0, sizeof(buffer));
  ctrlDataBuf = {};
}

double get_timestamp(struct msghdr& mhdr) {
  int level, type;
  struct timespec *ts = NULL;
  printf("%p\n", CMSG_FIRSTHDR(&mhdr));
  for (struct cmsghdr* cm = CMSG_FIRSTHDR(&mhdr); cm != NULL; cm = CMSG_NXTHDR(&mhdr, cm)) {
    printf("Hi\n");
    level = cm->cmsg_level;
    type  = cm->cmsg_type;
    if (SOL_SOCKET == level && SO_TIMESTAMPNS == type) {
      ts = (struct timespec *) CMSG_DATA(cm);
      printf("SW TIMESTAMP %ld.%09ld\n", (long)ts[0].tv_sec, (long)ts[0].tv_nsec);
    }
  }
  return 0;
}

void export_csv(std::vector<double>& all_times) {
  std::fstream fout;
  fout.open("all_times.csv", std::ios::out|std::ios::app);
  for (int i = 0; i < all_times.size(); i++) {
    fout << all_times.at(i) << "\n";
  }
}

int main() {
  int sockfd;
  struct msghdr mhdr;
  struct iovec iov[1];
  struct sockaddr_in servaddr, cliaddr;
  char buffer[MAXLINE];
  char ctrlDataBuf[MAXLINE*2];
  int timestampOn =
      SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_TX_SOFTWARE |
      SOF_TIMESTAMPING_SOFTWARE | SOF_TIMESTAMPING_RX_HARDWARE |
      SOF_TIMESTAMPING_TX_HARDWARE | SOF_TIMESTAMPING_RAW_HARDWARE;
  create_socket_fd(sockfd, timestampOn); 
  complete_sockaddr_info(servaddr, cliaddr);
  complete_message_info(mhdr, iov, servaddr, buffer, ctrlDataBuf);

  // Bind the socket with the server address 
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  }
  
  std::vector<double> all_times;
  int num_packets_received = 0;

  while (true) {
    int n = recvmsg(sockfd, &mhdr, MSG_WAITFORONE);
    buffer[n] = '\0';
    printf("Sanity check: %s\n", buffer);
    double timestamp = get_timestamp(mhdr);
    num_packets_received++;
    all_times.push_back(timestamp);
    if (num_packets_received > 1) {
      break;
    }
  }
  export_csv(all_times);
  close(sockfd);
  return 0;
}
