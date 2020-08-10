#include <skynet.h>
#include <skynet_socket.h>
#include <skynet_harbor.h>
#include <Windows.h>
void* get_function(const char* name) {
	HMODULE h = LoadLibraryA(0);
	if (h != INVALID_HANDLE_VALUE)
	{
		return GetProcAddress(h, name);
	}
	return 0;
}

int init_skynet_functions(skynet_functions* pfs) {
	get_skynet_functions_ptr p = 
		(get_skynet_functions_ptr)get_function("get_skynet_functions");
	if (p != 0) {
		if (p(pfs) != 0) {
			//ok
			return 1;
		}
	}
	return 0;
}
int init_skynet_socket_functions(skynet_socket_functions* ssf) {
	get_skynet_socket_functions_ptr p =
		(get_skynet_socket_functions_ptr)get_function("get_skynet_socket_functions");
	if (p != 0) {
		if (p(ssf) != 0) {
			return 1;
		}
	}
	return 0;
}
int init_skynet_harbor_functions(skynet_harbor_functions* ssf) {
	get_skynet_harbor_functions_ptr p =
		(get_skynet_harbor_functions_ptr)get_function("get_skynet_harbor_functions");
	if (p != 0) {
		if (p(ssf) != 0) {
			return 1;
		}
	}
	return 0;
}

char* strsep(char** stringp, const char* delim)
{
	char* start = *stringp;
	char* p = 0;
	p = (start != NULL) ? strpbrk(start, delim) : NULL;
	if (p == NULL)
	{
		*stringp = NULL;
	}
	else
	{
		*p = '\0';
		*stringp = p + 1;
	}
	return start;
}