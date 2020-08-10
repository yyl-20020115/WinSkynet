#ifndef SKYNET_HARBOR_H
#define SKYNET_HARBOR_H

#include <stdint.h>
#include <stdlib.h>

#define GLOBALNAME_LENGTH 16
#define REMOTE_MAX 256

struct remote_name {
	char name[GLOBALNAME_LENGTH];
	uint32_t handle;
};

struct remote_message {
	struct remote_name destination;
	const void * message;
	size_t sz;
	int type;
};

void skynet_harbor_send(struct remote_message *rmsg, uint32_t source, int session);
int skynet_harbor_message_isremote(uint32_t handle);
void skynet_harbor_init(int harbor);
void skynet_harbor_start(void * ctx);
void skynet_harbor_exit();

typedef void (*skynet_harbor_send_ptr)(struct remote_message* rmsg, uint32_t source, int session);
typedef int (*skynet_harbor_message_isremote_ptr)(uint32_t handle);
typedef void (*skynet_harbor_init_ptr)(int harbor);
typedef void (*skynet_harbor_start_ptr)(void* ctx);
typedef void (*skynet_harbor_exit_ptr)();

typedef struct _skynet_harbor_functions {
	skynet_harbor_init_ptr init_function;
	skynet_harbor_start_ptr start_function;
	skynet_harbor_exit_ptr exit_function;
	skynet_harbor_message_isremote_ptr message_isremote_function;
	skynet_harbor_send_ptr send_function;

}skynet_harbor_functions;

int get_skynet_harbor_functions(skynet_harbor_functions* shf);
typedef int (*get_skynet_harbor_functions_ptr)(skynet_harbor_functions* shf);
int init_skynet_harbor_functions(skynet_harbor_functions* shf);

#endif
