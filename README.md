请注意：


已经可以正常运行，但仍然需要更多测试以确认是否完全正确。
欢迎使用并指出BUG。
由于只是移植，而并非重新设计和实现，设计上的具体问题我可能无法解答，望见谅。



## ![skynet logo](https://github.com/cloudwu/skynet/wiki/image/skynet_metro.jpg)

Skynet is a lightweight online game framework which can be used in many other fields.

## Build

For Linux, install autoconf first for jemalloc:

```
git clone https://github.com/cloudwu/skynet.git
cd skynet
make 'PLATFORM'  # PLATFORM can be linux, macosx, freebsd now
```

Or:

```
export PLAT=linux
make
```

For FreeBSD , use gmake instead of make.

## Test

Run these in different consoles:

```
./skynet examples/config	# Launch first skynet node  (Gate server) and a skynet-master (see config for standalone option)
./3rd/lua/lua examples/client.lua 	# Launch a client, and try to input hello.
```

## About Lua version

Skynet now uses a modified version of lua 5.3.5 ( https://github.com/ejoy/lua/tree/skynet ) for multiple lua states.

Official Lua versions can also be used as long as the Makefile is edited.

## How To Use (Sorry, currently only available in Chinese)

* Read Wiki for documents https://github.com/cloudwu/skynet/wiki
* The FAQ in wiki https://github.com/cloudwu/skynet/wiki/FAQ
