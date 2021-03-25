#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>

enum {
	DEFDATALEN = 56,
	MAXIPLEN = 60,
	MAXICMPLEN = 76,
	MAXPACKET = 65468,
	MAX_DUP_CHK = (8 * 128),
	MAXWAIT = 10,
	PINGINTERVAL = 1, /* 1 second */
};

static int in_cksum(unsigned short *buf, int sz);

static void ping(const char *host)
{
	struct hostent *h;
	struct sockaddr_in pingaddr;
	struct icmp *pkt;
	int pingsock, c;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];
	static char *hostname;
	if ((pingsock = socket (AF_INET, SOCK_RAW, 1)) < 0) {
	// Create socket, 1 == ICMP/
	perror("ping: creating a raw socket");
	exit(1);
}
	pingaddr.sin_family = AF_INET;
	if (!(h = gethostbyname(host))) {
	fprintf(stderr, "ping: unknown host %s\n", host);
	exit(1);
}

memcpy(&pingaddr.sin_addr, h->h_addr,sizeof(pingaddr.sin_addr));
hostname = h->h_name;

//Build the icmp packet + checksum, send it
pkt = (struct icmp *) packet;
memset(pkt, 0, sizeof(packet));
pkt->icmp_type = ICMP_ECHO; //Set ICMP type
pkt->icmp_cksum = in_cksum((unsigned short *) pkt,sizeof(packet));
c = sendto(pingsock, packet, sizeof(packet), 0, (struct sockaddr *)&pingaddr, sizeof(struct sockaddr_in));

/* listen for replies */
while (1) {
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	if ((c = recvfrom(pingsock, packet, sizeof(packet), 0, (struct
	sockaddr *) &from, &fromlen)) < 0) {
	if (errno == EINTR)
	continue;
	perror("ping: recvfrom");
	continue;
	}
	if (c >= 76) {
	/* ip + icmp */
	struct iphdr *iphdr = (struct iphdr *) packet;
	pkt = (struct icmp *) (packet + (iphdr->ihl << 2));
	/* skip ip hdr */
	if (pkt->icmp_type == ICMP_ECHOREPLY)
	break;
	}
}
		printf("%s is alive!\n", hostname);
		return;
	}
		int main (int argc, char *argv[])
	{
	if (argc < 2)
	{
		fprintf(stderr,"usage %s hostname/IP\n", argv[0]);
		exit(0);
	}
	ping(argv[1]);

}
static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}
	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return ans;
}





