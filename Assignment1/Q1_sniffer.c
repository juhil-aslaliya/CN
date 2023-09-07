#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/tcp.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<errno.h>
#include<arpa/inet.h>

FILE *logfile;
struct sockaddr_in src, dest;

void printdata(unsigned char* data, int size) {
    int i, j;
    for(i == 0; i < size; i++) {
        if(i != 0 && i%16 == 0) {
            printf("         ");
            for(j = i-16; j<i; j++) {
                if(data[j] >= 32 && data[j]<=128) {
                    printf("%c", (unsigned char)data[j]);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
        if(i%16 == 0) {
            printf("    ");
        }
        printf(" %02X", (unsigned int)data[i]);
        if(i == size-1) {
            for(j=0; j < 15-i%16; j++) {
                printf("    ");
            }
            printf("         ");
            for(j=i-i%16; j<=i; j++) {
                if(data[j] >= 32 && data[j] <= 128) {
                    printf("%c", (unsigned char)data[j]);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}

void processpkt(unsigned char *buffer, int size) {
    unsigned short ipheaderlen;
    struct iphdr *iph = (struct iphdr *)buffer;
    ipheaderlen = iph->ihl*4;
    struct tcphdr *tcph = (struct tcphdr *)(buffer + ipheaderlen);
    memset(&src, 0, sizeof(src));
    src.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    printf("\n\n*************************************TCP Packet*************************************\n");
    printf("\n");
    printf("IP Header\n");
	printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
	printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
	printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
	printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
	printf("   |-Identification    : %d\n",ntohs(iph->id));
	printf("   |-TTL      : %d\n",(unsigned int)iph->ttl);
	printf("   |-Protocol : %d\n",(unsigned int)iph->protocol);
	printf("   |-Checksum : %d\n",ntohs(iph->check));
	printf("   |-Source IP        : %s\n",inet_ntoa(src.sin_addr));
	printf("   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
    printf("\n");
	printf("TCP Header\n");
	printf("   |-Source Port      : %u\n",ntohs(tcph->source));
	printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
	printf("   |-Sequence Number    : %u\n",ntohl(tcph->seq));
	printf("   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
	printf("   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
	printf("   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
	printf("   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
	printf("   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
	printf("   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
	printf("   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
	printf("   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
	printf("   |-Window         : %d\n",ntohs(tcph->window));
	printf("   |-Checksum       : %d\n",ntohs(tcph->check));
	printf("   |-Urgent Pointer : %d\n",tcph->urg_ptr);
	printf("\n");
	printf("                        DATA Dump                         ");
	printf("\n");
    printf("IP Header\n");
    printdata(buffer, ipheaderlen);
    printf("TCP Header\n");
    printdata(buffer+ipheaderlen, tcph->doff*4);
    printf("Data Payload\n");
    printdata(buffer+ipheaderlen+tcph->doff*4, (size - tcph->doff*4 - iph->ihl*4));
    printf("####################################################################################");
}

int main() {

    printf("Address of logfile: %d \n", (int)&logfile);
    int s;
    struct sockaddr saddr;
    int saddr_size;
    size_t k;
    unsigned char *buffer = (unsigned char *) malloc(65536);
    ssize_t rcv;

    s = socket(2, 3, 6);
    printf("Socket succeccfully established %d \n", s);
    if(s < 0) {
        perror("socket");
        return 1;
    }

    while(1){
        saddr_size = sizeof saddr;
        rcv = recvfrom(s, buffer, 65536, 0, &saddr, &saddr_size);
        if(rcv < 0) {
            printf("Nay! no packets :( \n");
            break;
        }
        processpkt(buffer, rcv);
    }
    close(s);
    printf("Khatam, Tata, Goodbye! \n");
    return 0;
}
