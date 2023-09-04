#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/tcp.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<errno.h>

int main() {

    int s;
    s = socket(2, 3, 6);
    printf("Socket succeccfully established %d \n", s);
    if(s < 0) {
        perror("socket");
    }

    struct sockaddr saddr;
    int saddr_size;
    saddr_size = sizeof saddr;
    size_t k;
    unsigned char *buffer = (unsigned char *) malloc(65536);
    ssize_t rcv;
    rcv = recvfrom(s, buffer, 65536, 0, &saddr, &saddr_size);
    if(rcv>0) {
        printf("Yay! \n");
    } else {
        printf("Nay! \n");
    }
    return 0;
}
