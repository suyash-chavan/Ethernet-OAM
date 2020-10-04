#define his_port "8080"

#define max_socket_retries 4
#include "../../his_ip.h"
int send_socket(char *send_buf)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	int s, sock, j;

	size_t len;
	ssize_t nread;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	s = getaddrinfo(his_ip, his_port, &hints, &result);

	if (s != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
/*	
	for(rp=result;rp!=NULL;rp=rp->ai_next)
	{
		sock=socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);

		if(sock==-1)
			continue;

		if(connect(sock,rp->ai_addr,rp->ai_addrlen)!=-1)
			break;

		close(sock);
	}

  */
	    rp = result;
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (sock == -1)
		{
			printf("Socket Failed\n");
			exit(EXIT_FAILURE);
		}
			
		int tries=max_socket_retries;
        
        do
        {
		    if (connect(sock, rp->ai_addr, rp->ai_addrlen)!= -1)
			    break;
			sleep(1);
			printf("Tring once again...Tries = %d\n",tries);
			tries--;
        }
        while(tries>0);
		

	if (rp == NULL)
	{
		fprintf(stderr, "Could Not Connect\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);

	printf("SENDING THESE BYTES THROUGH SOCKET = %d\n",(int)write(sock, send_buf, OAMPDU_MAX));

}
