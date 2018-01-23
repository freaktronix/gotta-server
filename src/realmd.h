#ifndef REALMD_H
#define REALMD_H

#include <stdbool.h>

#define REALMD_VERSION "0.0.1"

#define REALMD_PORT "17420"
#define REALMD_SOCKET_QUEUE 128

typedef struct realmd_peer_t realmd_peer_t;

typedef struct {
	int sock;
	realmd_peer_t *peers;
} realmd_listener_t;

struct realmd_peer_t {
	int sock;
	struct sockaddr_in *client;
	int addrlen;
	realmd_peer_t *next;
};

void realmd_loop(realmd_listener_t *gotta);
bool realmd_new_peer(realmd_listener_t *gotta, realmd_peer_t *peer);
int realmd_start_listen(realmd_listener_t *gotta);

#endif
