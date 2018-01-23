#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "realmd.h"

void realmd_loop(realmd_listener_t *gotta)
{
	realmd_peer_t peer;
	bool running = true;

	while(running) {
		peer.sock = accept(
				gotta->sock,
				(struct sockaddr *) &peer.client,
				(socklen_t *) &peer.addrlen);

		if(peer.sock >= 0)
		{
			realmd_new_peer(gotta, &peer);
			fprintf(stdout, "> new peer is connected\n");
		}
	}
}

bool realmd_new_peer(realmd_listener_t *gotta, realmd_peer_t *peer)
{
	realmd_peer_t *new_peer = malloc(sizeof(realmd_peer_t));
	struct sockaddr_in *client = malloc(sizeof(struct sockaddr_in));

	if(new_peer == NULL)
	{
		return false;
	}
	else
	{
		if(client == NULL)
		{
			free(new_peer);
			return false;
		}
	}

	memset(new_peer, 0, sizeof(realmd_peer_t));
	memset(client, 0, sizeof(struct sockaddr_in));
	memcpy(client, peer->client, sizeof(struct sockaddr_in));
	memcpy(new_peer, peer, sizeof(realmd_peer_t));

	new_peer->client = client;

	if(gotta->peers == NULL)
	{
		new_peer->next = NULL;
		gotta->peers = new_peer;
	}
	else
	{
		new_peer->next = gotta->peers;
		gotta->peers = new_peer;
	}

	fprintf(stdout, "> peer is subscribed\n");
	return true;
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
	fprintf(stdout, "> starting realmd...\n");

	if(realmd_start_listen(&gotta) >= 0)
	{
		fprintf(stdout, "> listening on port %s\n", REALMD_PORT);
		realmd_loop(&gotta);
		close(gotta.sock);
	}

	return 0;
}
