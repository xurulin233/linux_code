#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>

#define BUFFER_SIZE 2048

int main() {
    int sockfd, n;
    char buffer[BUFFER_SIZE];
    struct sockaddr_ll sa;
    struct ifreq ifr; // For setting promiscuous mode
    char *interface_name = "ens33";

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Enable promiscuous mode
    strncpy(ifr.ifr_name, interface_name, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1) {
        perror("ioctl SIOCGIFFLAGS");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    ifr.ifr_flags |= IFF_PROMISC;
    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) == -1) {
        perror("ioctl SIOCSIFFLAGS");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ALL);
    sa.sll_ifindex = if_nametoindex(interface_name);
    if (bind(sockfd, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Listening on %s in promiscuous mode...\n", interface_name);
    
    while (1) {
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (n == -1) {
            perror("recvfrom");
            break;
        }
        printf("Received a frame of length: %d bytes\n", n);
    }
    
    close(sockfd);
    return 0;
}

