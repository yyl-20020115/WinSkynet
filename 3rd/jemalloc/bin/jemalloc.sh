#!/bin/sh

prefix=/usr/local
exec_prefix=/usr/local
libdir=${exec_prefix}/lib

LD_PRELOAD=${libdir}/libjemalloc.dll
export LD_PRELOAD
exec "$@"
