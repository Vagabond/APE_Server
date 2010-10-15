/*
  Copyright (C) 2006, 2007, 2008, 2009, 2010  Anthony Catel <a.catel@weelya.com>
  Copyright (C) 2010 Andrew Thompson <andrew@hijacked.us>

  This file is part of APE Server.
  APE is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  APE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with APE ; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/* ape_zmq.h */

#ifndef _ape_zmq_H
#define _ape_zmq_H

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>

#include <zmq.h>

#include "plugins.h"
#include "global_plugins.h"

struct _ape_zmq_socket
{
	void *zmq_socket;

	struct {
		void (*on_message)(void *message, acetables *g_ape);
		void (*on_multipart_message)(void **messages, size_t count, acetables *g_ape);
		void (*on_write)(void *zmq_socket, acetables *g_ape);
	} callbacks;
};

typedef struct _ape_zmq_socket ape_zmq_socket;

void ape_zmq_init(acetables *g_ape);
void ape_zmq_term(acetables *g_ape);
ape_zmq_socket *ape_zmq_connect(void *zmq_socket, const char *endpoint, acetables *g_ape);
ape_zmq_socket *ape_zmq_bind(void *zmq_socket, const char *endpoint, acetables *g_ape);
void *ape_zmq_socket_new(int type);

#endif
