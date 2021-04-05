//
// Created by peilun on 2/24/20.
//
// UDP client


#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctime>
using namespace std;

#define PORT 4783
// #define MSG_CONFIRM 0
#define MSG_LOSS "Error: Message Lost!"
struct timeval start,last,latency;


int main() {
    int sockfd, n;
    socklen_t len;
    char buffer[1024];
    const char* hello = "Client : hello server!";
    struct sockaddr_in servaddr;
    // Create a UDP socket
    // Notice the use of SOCK_DGRAM for UDP packets
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    fd_set select_fds;
    // select() set up
    FD_ZERO(&select_fds);
    FD_SET(sockfd, &select_fds);
    //set the waitting time of resend the message
    latency.tv_sec = 1;
    latency.tv_usec = 0;

    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
    servaddr.sin_port = htons(PORT); // port number

    int i = 0;
    while (i<10) {
        double t1=0.0,t2=0.0;
        //record the time of PC which message start to send
        gettimeofday(&start,NULL);
        //send the message to server
        sendto(sockfd, hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

        if (select(64, &select_fds, NULL, NULL, &latency) == 0){
                cout<<"message send time out,waitting for next"<<endl;
        }
        else{
        //Receive the server packet along with the address it is coming from
        n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
        // rtt end
        //record the time of PC which message receive successful
        gettimeofday(&last,NULL);

        double r1= last.tv_sec+(last.tv_usec/10e6);
        double r2= start.tv_sec+(start.tv_usec/10e6);
        double RTT = r1 -r2;
        // print the message from server
        buffer[n] = '\0';
        cout<<buffer<<endl;
        cout<<"RTT:"<<RTT<<endl;
        }
        i++;
    }
    close(sockfd);
    return 0;
}
