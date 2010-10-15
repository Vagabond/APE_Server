include ./plateform.mk
include ./mysql.mk
include ./zmq.mk

CC=gcc -D_GNU_SOURCE

ifeq ($(HAS_MYSQL), yes)
	MYSQL_FLAGS = -L./deps/mysac/ -I./deps/mysac/ -lmysac -lmysqlclient_r
endif

MODULES=lib/libmod_spidermonkey.so

ifeq ($(HAS_ZMQ), 1)
	MODULES += lib/libmod_zeromq.so
endif

all: modules

modules: $(MODULES)

ifdef DARWIN_BUILD

lib/libmod_spidermonkey.so: libape-spidermonkey.c
	$(CC) -Wall -g -bundle -undefined suppress -flat_namespace -o lib/libmod_spidermonkey.so libape-spidermonkey.c -I../deps/js/src/dist/include/ -L../deps/js/src/ -ljs_static -lstdc++ $(MYSQL_FLAGS)

lib/libmod_zeromq.so: libape-zeromq.c
	$(CC) -Wall -g -bundle -undefined suppress -flat_namespace -o lib/libmod_zeromq.so libape-zeromq.c -lzmq

endif
ifdef LINUX_BUILD

CFLAGS = -Wall -O2 -shared -fPIC -rdynamic

lib/libmod_spidermonkey.so: libape-spidermonkey.c
	$(CC) $(CFLAGS) -Wl,-soname,libmod_spidermonkey.so -o lib/libmod_spidermonkey.so libape-spidermonkey.c -I../deps/js/src/dist/include/ -L../deps/js/src/ -ljs_static -lstdc++ $(MYSQL_FLAGS)

lib/libmod_zeromq.so: libape-zeromq.c
	$(CC) $(CFLAGS) -Wl,-soname,libmod_zeromq.so -o lib/libmod_zeromq.so libape-zeromq.c -lzmq
	
endif
