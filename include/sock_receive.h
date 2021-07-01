#define my_port "8080"

#define OAMPDU_MAX 1600

struct addrinfo hints;
struct addrinfo *result, *rp;
int sfd, s;
struct sockaddr_storage peer_addr;
socklen_t peer_addr_len;
ssize_t nread;

void receive_socket_init()
{

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	s = getaddrinfo(NULL, my_port, &hints, &result);

	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
		             rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(sfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
}


void receive_socket(char* receive_buf)
{
	peer_addr_len = sizeof(struct sockaddr_storage);
	nread = recvfrom(sfd, receive_buf, OAMPDU_MAX, 0,
	                 (struct sockaddr *) &peer_addr, &peer_addr_len);
	                 
	printf("***NREAD: %d\n",(int)nread);
	if (nread == -1)
		return ;            /* Ignore failed request */

	char host[NI_MAXHOST], service[NI_MAXSERV];

	s = getnameinfo((struct sockaddr *) &peer_addr,
	                peer_addr_len, host, NI_MAXHOST,
	                service, NI_MAXSERV, NI_NUMERICSERV);

	if (s != 0)
	{
		fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

}
