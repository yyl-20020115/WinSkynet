#pragma once

#ifdef _WIN32
#include <skynet.h>
#include <skynet_socket.h>
#include <skynet_harbor.h>
skynet_functions sfs = { 0 };
skynet_socket_functions ssf={ 0 };
skynet_harbor_functions shf = { 0 };

#define skynet_sendname sfs.sendname_function
#define skynet_command sfs.command_function
#define skynet_callback sfs.call_back_function
#define skynet_error sfs.error_function
#define skynet_lalloc sfs.lalloc_function
#define skynet_send sfs.send_function
#define skynet_queryname sfs.queryname_function

#define skynet_socket_close ssf.close_function
#define skynet_socket_start ssf.start_function
#define skynet_socket_listen ssf.listen_function
#define skynet_socket_sendbuffer ssf.sendbuffer_function
//#define skynet_socket_send ssf.send_function

#define skynet_harbor_start shf.start_function

#endif
