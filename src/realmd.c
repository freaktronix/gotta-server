#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define REALMD_VERSION "0.0.1"

#define REALMD_PORT "17420"
#define REALMD_SOCKET_QUEUE 128

typedef struct {
	int sock;
} realmd_listener_t;

typedef struct {
	int sock;
} realmd_peer_t;

void realmd_loop(realmd_listener_t *gotta)
{
}

int realmd_start_listen(realmd_listener_t *gotta)
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
	realmd_listener_t gotta;

	fprintf(stdout, "             _   _\n");
	fprintf(stdout, "  __ _  ___ | |_| |_ __ _\n");
	fprintf(stdout, " / _` |/ _ \\| __| __/ _` |\n");
	fprintf(stdout, "| (_| | (_) | |_| || (_| |\n");
	fprintf(stdout, " \\__, |\\___/ \\__|\\__\\__,_|\n");
	fprintf(stdout, " |___/ version: %s\n\n", REALMD_VERSION);
	fprintf(stdout, "> starting server...\n");

	if(realmd_start_listen(&gotta) >= 0)
	{
		fprintf(stdout, "> listening on port %s\n", REALMD_PORT);
		realmd_loop(&gotta);
		close(gotta.sock);
	}

	return 0;
}
