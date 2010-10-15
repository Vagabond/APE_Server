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

/* ape_zmq.c */

#include "libape-zeromq.h"

#define MODULE_NAME "zeromq"

static ace_plugin_infos infos_module = {
	"ZeroMQ APE interface",  // Module Name
	"0.01",                  // Module Version
	"Andrew Thompson",       // Module Author
	NULL                     // Config file
};

static void *zmq_context;

static void ape_zmq_read(ape_socket *client, ape_buffer *buf, size_t offset, acetables *g_ape) {
	zmq_msg_t msg;
	void *data;
	ape_zmq_socket *ape_zmq_sock;

	ape_zmq_sock = (ape_zmq_socket*) client->data;

	while (1) {
		int rc = zmq_msg_init(&msg);
		assert(rc == 0);

		if (zmq_recv(ape_zmq_sock->zmq_socket, &msg, ZMQ_NOBLOCK)) {
			zmq_msg_close(&msg);
			break;
		}

		data = zmq_msg_data(&msg);

		if (ape_zmq_sock->callbacks.on_message != NULL) {
			ape_zmq_sock->callbacks.on_message(data, g_ape);
		}

		zmq_msg_close(&msg);
	}
}

static void ape_zmq_write(ape_socket *client, acetables *g_ape) {
}

static ape_zmq_socket *ape_zmq_socket_prepare(void *zmq_socket, acetables *g_ape) {
	int res;
	int sock;
	size_t sock_size = sizeof(int);
	ape_zmq_socket *ape_zmq_sock;

	res = zmq_getsockopt(zmq_socket, ZMQ_FD, &sock, &sock_size);
	/* this shouldn't really fail, so lets assert it doesn't */
	assert(res == 0);

	prepare_ape_socket(sock, g_ape);

	g_ape->co[sock]->fd = sock;
	g_ape->co[sock]->state = STREAM_PROGRESS;
	g_ape->co[sock]->stream_type = STREAM_DELEGATE;

	ape_zmq_sock = xmalloc(sizeof(*ape_zmq_sock));
	ape_zmq_sock->zmq_socket = zmq_socket;

	g_ape->co[sock]->data = ape_zmq_sock;

	g_ape->bufout[sock].fd = sock;

	g_ape->co[sock]->callbacks.on_read = ape_zmq_read;
	g_ape->co[sock]->callbacks.on_write = ape_zmq_write;

	events_add(g_ape->events, sock, EVENT_READ|EVENT_WRITE);

	return g_ape->co[sock]->data;
}

ape_zmq_socket *ape_zmq_connect(void *zmq_socket, const char *endpoint, acetables *g_ape) {
	if (zmq_connect(zmq_socket, endpoint)) {
		return NULL;
	}
	return ape_zmq_socket_prepare(zmq_socket, g_ape);
}

ape_zmq_socket *ape_zmq_bind(void *zmq_socket, const char *endpoint, acetables *g_ape) {
	if (zmq_bind(zmq_socket, endpoint)) {
		return NULL;
	}
	return ape_zmq_socket_prepare(zmq_socket, g_ape);
}

void *ape_zmq_socket_new(int type) {
	if (!zmq_context) {
		printf("zmq init\n");
		zmq_context = zmq_init(1);
		assert(zmq_context != NULL);
	}
	return zmq_socket(zmq_context, type);
}

static void init_module(acetables *g_ape) {
}

static void free_module(acetables *g_ape) {
	//zmq_term(zmq_context);
}

static ace_callbacks callbacks = {
	NULL,              /* Called when new user is added */
	NULL,              /* Called when a user is disconnected */
	NULL,              /* Called when new chan is created */
	NULL,              /* Called when a chan is deleted */
	NULL,              /* Called when a user join a channel */
	NULL,              /* Called when a user leave a channel */
	NULL,              /* Called when doing a user tick */
	NULL,              /* Called when posting a raw to a subuser */
	NULL,              /* Called when allocating a user */
	NULL,              /* Called when adding a subuser */
	NULL               /* Called when deleting a subuser */
};

APE_INIT_PLUGIN(MODULE_NAME, init_module, free_module, callbacks)
