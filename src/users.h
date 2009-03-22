/*
  Copyright (C) 2006, 2007, 2008, 2009  Anthony Catel <a.catel@weelya.com>

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

/* users.h */

#ifndef _USERS_H
#define _USERS_H

#include "main.h"

#include "channel.h"
#include "json.h"
#include "extend.h"


#define FLG_NOFLAG	0x00
#define FLG_AUTOOP 	0x01
#define FLG_NOKICK 	0x02
#define FLG_BOTMANAGE 	0x04
#define FLG_PCONNECT 	0x08


#define MAX_SESSION_LENGTH 102400 // 100 ko
#define MAX_HOST_LENGTH 256


enum {
	TRANSPORT_LONGPOLLING,
	TRANSPORT_POLLING,
	TRANSPORT_JSONP,
	TRANSPORT_IFRAME
};

// Le 25/12/2006 � 02:15:19 Joyeux No�l

typedef struct USERS
{

	unsigned int nraw;
	unsigned int flags;
	unsigned int type;

	char sessid[33];
	

	char ip[16]; // ipv4
	
	long int idle;
	
	int transport;
	
	struct USERS *next;
	struct USERS *prev;
	
	struct CHANLIST *chan_foot;

	struct {
		int length;
		struct _session *data;
	} sessions;
	
	struct _transpipe *pipe;
	
	struct {
		int nlink;
		struct _link_list *ulink;
	} links;
	struct _extend *properties;

	
	struct _subuser *subuser;
	int nsub;
	
	char lastping[24];

} USERS;

typedef struct _subuser subuser;

struct _subuser
{
	int fd;
	int state;
	int headers_sent;
	
	int need_update;
	
	/* In case of subuser socket is still marked as ALIVE and timed out */
	int wait_for_free;
	
	/* Channel can be identified by Host HTTP Header */
	char channel[MAX_HOST_LENGTH+1];
	
	struct _subuser *next;
	USERS *user;
	
	int nraw;
	struct RAW *rawhead;
	struct RAW *rawfoot;
	
	long int idle;
};


typedef struct CHANLIST
{
	struct CHANNEL *chaninfo;
	struct CHANLIST *next;
	
} CHANLIST;


struct _users_link
{
	USERS *a;
	USERS *b;
	
	int link_type;
};

struct _link_list
{
	struct _users_link *link;
	struct _link_list *next;
};


typedef struct userslist
{
	struct USERS *userinfo;
	struct userslist *next;
		
	unsigned int level;
	/* TODO: it can be intersting to extend this */
} userslist;

typedef struct RAW
{
	char *data;
	int len;
	struct RAW *next;
	int priority;
} RAW;


typedef struct _session session;

struct _session
{
	char key[33];
	char *val;
	struct _session *next;
};


enum {
	ALIVE = 0,
	ADIED
};

enum {
	HUMAN = 0,
	BOT
};

enum {
	PRIVMSG = 0,
	CHANMSG,
	INFOMSG
};
enum {
	RAW_MSG = 0,
	RAW_LOGIN,
	RAW_JOIN,
	RAW_LEFT,
	RAW_SETLEVEL
};

#define RAW_DATA 		"DATA"
#define RAW_CHANMSG 		"CHANMSG"
#define RAW_TOPIC 		"TOPIC"
#define RAW_LOGIN 		"LOGIN"
#define RAW_JOIN 		"JOIN"
#define RAW_LEFT 		"LEFT"
#define RAW_SETLEVEL 		"SETLVL"
#define RAW_SETTOPIC 		"SETTOPIC"
#define RAW_USER 		"USER"
#define RAW_ERR 		"ERR"
#define RAW_CHANNEL		"CHANNEL"
#define RAW_KICK		"KICKED"
#define RAW_BAN			"BANNED"
#define RAW_PROXY		"PROXY"

USERS *seek_user(char *nick, char *linkid, acetables *g_ape);
USERS *init_user(acetables *g_ape);
USERS *adduser(unsigned int fdclient, char *host, acetables *g_ape);
USERS *seek_user_id(char *sessid, acetables *g_ape);
USERS *seek_user_simple(char *nick, acetables *g_ape);


RAW *forge_raw(char *raw, struct json *jlist);
RAW *copy_raw(RAW *input);

void post_raw(RAW *raw, USERS *user);
void post_raw_sub(RAW *raw, subuser *sub);
void post_raw_channel(RAW *raw, struct CHANNEL *chan);
void post_raw_restricted(RAW *raw, USERS *user, subuser *sub);
void post_raw_channel_restricted(RAW *raw, struct CHANNEL *chan, USERS *ruser);


void send_raws(subuser *user);

void deluser(USERS *user, acetables *g_ape);

void do_died(subuser *user);

void check_timeout(acetables *g_ape);
void grant_aceop(USERS *user);

void send_error(USERS *user, char *msg, char *code);
void send_msg(USERS *user, char *msg, char *type);
void send_msg_sub(subuser *sub, char *msg, char *type);
void send_msg_channel(CHANNEL *chan, char *msg, char *type);

unsigned int isonchannel(USERS *user, CHANNEL *chan);

struct json *get_json_object_user(USERS *user);

session *get_session(USERS *user, char *key);
session *set_session(USERS *user, char *key, char *val, int update);
void clear_sessions(USERS *user);
void sendback_session(USERS *user, session *sess);

subuser *addsubuser(int fd, char *channel, USERS *user);
subuser *getsubuser(USERS *user, char *channel);
void delsubuser(subuser **current);

void clear_subusers(USERS *user);
void clear_subuser_raws(subuser *sub);
void ping_request(USERS *user);

void make_link(USERS *a, USERS *b);
struct _users_link *are_linked(USERS *a, USERS *b);
void destroy_link(USERS *a, USERS *b);

int post_to_pipe(json *jlist, char *rawname, char *pipe, subuser *from, void *restrict, acetables *g_ape);

#endif

