#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

// Function prototypes
void process_packet(unsigned char *buffer, int size);
void print_ethernet_header(unsigned char *buffer);
void print_ip_header(unsigned char *buffer);
void print_tcp_packet(unsigned char *buffer, int size);
void print_udp_packet(unsigned char *buffer, int size);
void print_icmp_packet(unsigned char *buffer, int size);
void print_payload(unsigned char *buffer, int size);

// Packet statistics
int packet_count = 0;
int tcp_count = 0, udp_count = 0, icmp_count = 0, others_count = 0;

int main() {
    int raw_sock;
    int data_size;
    unsigned char buffer[65536];

    printf("====================================================\n");
    printf("        RAW SOCKET PACKET CAPTURE APPLICATION       \n");
    printf("====================================================\n");

    // Create a raw socket that listens to all Ethernet frames (ETH_P_ALL)
    raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_sock < 0) {
        perror("Socket Creation Failed (Make sure to run as root / sudo)");
        return 1;
    }

    printf("Capturing packets... Press Ctrl+C to stop.\n\n");

    // Main packet capture loop
    while (1) {
        // Receive packet data from socket
        data_size = recvfrom(raw_sock, buffer, sizeof(buffer), 0, NULL, NULL);
        if (data_size < 0) {
            perror("Failed to receive packets");
            close(raw_sock);
            return 1;
        }

        packet_count++;
        process_packet(buffer, data_size);
    }

    close(raw_sock);
    return 0;
}

// Process captured packet and route to specific protocol handlers
void process_packet(unsigned char *buffer, int size) {
    // Extract IP header (located right after 14-byte Ethernet header)
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    // Check Ethernet layer protocol (Must be IP package: 0x0800)
    struct ethhdr *eth = (struct ethhdr *)buffer;
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        others_count++;
        return; // Ignore non-IP packets (like ARP, IPv6, etc.) to keep simple
    }

    printf("\n====================================================\n");
    printf(" PACKET #%d | Total Size: %d Bytes\n", packet_count, size);
    printf("====================================================\n");

    // 1. Display Ethernet Header details
    print_ethernet_header(buffer);

    // 2. Display IP Header details
    print_ip_header(buffer);

    // 3. Display Transport Layer Header details based on IP protocol
    switch (iph->protocol) {
        case IPPROTO_TCP: // Protocol 6
            tcp_count++;
            print_tcp_packet(buffer, size);
            break;

        case IPPROTO_UDP: // Protocol 17
            udp_count++;
            print_udp_packet(buffer, size);
            break;

        case IPPROTO_ICMP: // Protocol 1
            icmp_count++;
            print_icmp_packet(buffer, size);
            break;

        default:
            others_count++;
            printf("\n--- [ Protocol: Unknown / Other (%d) ] ---\n", iph->protocol);
            break;
    }

    printf("----------------------------------------------------\n");
    printf(" Stats -> Total: %d | TCP: %d | UDP: %d | ICMP: %d | Other: %d\n",
           packet_count, tcp_count, udp_count, icmp_count, others_count);
}

// Display Ethernet Header Details
void print_ethernet_header(unsigned char *buffer) {
    struct ethhdr *eth = (struct ethhdr *)buffer;

    printf("\n--- [ ETHERNET HEADER ] ---\n");
    printf(" Destination MAC : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
           eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf(" Source MAC      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
           eth->h_source[0], eth->h_source[1], eth->h_source[2],
           eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf(" Protocol        : 0x%04X (IP)\n", ntohs(eth->h_proto));
}

// Display IP Header Details
void print_ip_header(unsigned char *buffer) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));

    struct sockaddr_in src_addr, dst_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    memset(&dst_addr, 0, sizeof(dst_addr));

    src_addr.sin_addr.s_addr = iph->saddr;
    dst_addr.sin_addr.s_addr = iph->daddr;

    printf("\n--- [ IP HEADER ] ---\n");
    printf(" IP Version        : %d\n", (unsigned int)iph->version);
    printf(" Header Length     : %d Bytes (%d DWORDS)\n", ((unsigned int)(iph->ihl)) * 4, (unsigned int)iph->ihl);
    printf(" Type of Service   : %d\n", (unsigned int)iph->tos);
    printf(" Total Length      : %d Bytes\n", ntohs(iph->tot_len));
    printf(" Identification    : %d\n", ntohs(iph->id));
    printf(" Time To Live (TTL): %d\n", (unsigned int)iph->ttl);
    printf(" Protocol          : %d\n", (unsigned int)iph->protocol);
    printf(" Checksum          : 0x%04X\n", ntohs(iph->check));
    printf(" Source IP         : %s\n", inet_ntoa(src_addr.sin_addr));
    printf(" Destination IP    : %s\n", inet_ntoa(dst_addr.sin_addr));
}

// Display TCP Packet Details
void print_tcp_packet(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    unsigned short ip_hdr_len = iph->ihl * 4;

    struct tcphdr *tcph = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + ip_hdr_len);
    unsigned short tcp_hdr_len = tcph->doff * 4;

    printf("\n--- [ TCP HEADER ] ---\n");
    printf(" Source Port       : %d\n", ntohs(tcph->source));
    printf(" Destination Port  : %d\n", ntohs(tcph->dest));
    printf(" Sequence Number   : %u\n", ntohl(tcph->seq));
    printf(" Acknowledge Number: %u\n", ntohl(tcph->ack_seq));
    printf(" Header Length     : %d Bytes\n", tcp_hdr_len);
    printf(" Flags             : ");
    if (tcph->urg) printf("URG ");
    if (tcph->ack) printf("ACK ");
    if (tcph->psh) printf("PSH ");
    if (tcph->rst) printf("RST ");
    if (tcph->syn) printf("SYN ");
    if (tcph->fin) printf("FIN ");
    printf("\n");
    printf(" Window Size       : %d\n", ntohs(tcph->window));
    printf(" Checksum          : 0x%04X\n", ntohs(tcph->check));
    printf(" Urgent Pointer    : %d\n", tcph->urg_ptr);

    // Calculate payload start & payload size
    int header_size = sizeof(struct ethhdr) + ip_hdr_len + tcp_hdr_len;
    int payload_size = size - header_size;
    print_payload(buffer + header_size, payload_size);
}

// Display UDP Packet Details
void print_udp_packet(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    unsigned short ip_hdr_len = iph->ihl * 4;

    struct udphdr *udph = (struct udphdr *)(buffer + sizeof(struct ethhdr) + ip_hdr_len);

    printf("\n--- [ UDP HEADER ] ---\n");
    printf(" Source Port       : %d\n", ntohs(udph->source));
    printf(" Destination Port  : %d\n", ntohs(udph->dest));
    printf(" UDP Length        : %d Bytes\n", ntohs(udph->len));
    printf(" Checksum          : 0x%04X\n", ntohs(udph->check));

    // Calculate payload start & payload size
    int header_size = sizeof(struct ethhdr) + ip_hdr_len + sizeof(struct udphdr);
    int payload_size = size - header_size;
    print_payload(buffer + header_size, payload_size);
}

// Display ICMP Packet Details
void print_icmp_packet(unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    unsigned short ip_hdr_len = iph->ihl * 4;

    struct icmphdr *icmph = (struct icmphdr *)(buffer + sizeof(struct ethhdr) + ip_hdr_len);

    printf("\n--- [ ICMP HEADER ] ---\n");
    printf(" Type     : %d ", (unsigned int)(icmph->type));
    if ((unsigned int)(icmph->type) == ICMP_ECHOREPLY) printf("(Echo Reply)\n");
    else if ((unsigned int)(icmph->type) == ICMP_ECHO) printf("(Echo Request)\n");
    else printf("\n");

    printf(" Code     : %d\n", (unsigned int)(icmph->code));
    printf(" Checksum : 0x%04X\n", ntohs(icmph->checksum));

    // Calculate payload start & payload size
    int header_size = sizeof(struct ethhdr) + ip_hdr_len + sizeof(struct icmphdr);
    int payload_size = size - header_size;
    print_payload(buffer + header_size, payload_size);
}

// Display Payload Data in Hex & ASCII format
void print_payload(unsigned char *data, int size) {
    printf("\n--- [ PAYLOAD DATA (%d Bytes) ] ---\n", size);
    if (size <= 0) {
        printf(" (No Payload / Header-only Packet)\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        // Print hex byte
        printf("%02X ", data[i]);

        // Print ASCII characters at the end of each 16-byte line
        if ((i + 1) % 16 == 0 || i == size - 1) {
            // Pad remaining spaces if line is shorter than 16 bytes
            int pad = 15 - (i % 16);
            for (int j = 0; j < pad; j++) {
                printf("   ");
            }
            printf(" | ");

            // Print ASCII representation of the line
            int start_idx = i - (i % 16);
            for (int j = start_idx; j <= i; j++) {
                if (data[j] >= 32 && data[j] <= 126) {
                    printf("%c", data[j]); // Printable character
                } else {
                    printf("."); // Non-printable character
                }
            }
            printf("\n");
        }
    }
}
