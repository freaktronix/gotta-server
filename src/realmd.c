#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define REALMD_PORT "17420"
#define REALMD_SOCKET_QUEUE 128

typedef struct {
	int sock;
} realmd_socket_t;

int realmd_start_listen(realmd_socket_t *gotta)
{
	struct addrinfo hints, *res, *resSave;
	int n;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	n = getaddrinfo("*", REALMD_PORT, &hints, &res);
	gotta->sock = -1;

	if(n >= 0)
	{
		resSave = res;
		while(res)
		{
			gotta->sock = socket(
					res->ai_family,
					res->ai_socktype,
					res->ai_protocol);
			if(gotta->sock >= 0)
			{
				if(bind(gotta->sock, res->ai_addr, res->ai_addrlen) == 0)
					break;

				close(gotta->sock);
				gotta->sock = -1;
			}

			res = res->ai_next;
		}

		if(gotta->sock >= 0)
		{
			listen(gotta->sock, REALMD_SOCKET_QUEUE);
		}

		freeaddrinfo(resSave);
	}

	return gotta->sock;
}

int main()
{
	realmd_socket_t gotta;

	if(realmd_start_listen(&gotta) >= 0)
	{
		fprintf(stdout, "Starting GOTTA server...");
		close(gotta.sock);
	}

	return 0;
}
