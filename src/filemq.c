/*  =========================================================================
    filemq - FileMQ command-line service

    -------------------------------------------------------------------------
    Copyright (c) 1991-2012 iMatix Corporation <www.imatix.com>
    Copyright other contributors as noted in the AUTHORS file.

    This file is part of FILEMQ, see http://filemq.org.

    This is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    This software is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTA-
    BILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
    Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
    =========================================================================
*/

#include "czmq.h"
#include "../include/fmq.h"

#define PRODUCT         "FileMQ service/1.0a1"
#define COPYRIGHT       "Copyright (c) 2012 iMatix Corporation"
#define NOWARRANTY \
"This is free software; see the source for copying conditions.  There is NO\n" \
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n" 

int main (int argc, char *argv [])
{
    puts (PRODUCT);
    puts (COPYRIGHT);
    puts (NOWARRANTY);

    fmq_server_t *server = NULL;
    fmq_client_t *client = NULL;

    if (argc < 2 || streq (argv [1], "-s")) {
        server = fmq_server_new ();
        fmq_server_configure (server, "server_test.cfg");
        fmq_server_publish (server, "./fmqroot/send", "/");
        fmq_server_publish (server, "./fmqroot/logs", "/logs");
        //  We do this last
        fmq_server_bind (server, "tcp://*:6000");
    }
    if (argc < 2 || streq (argv [1], "-c")) {
        client = fmq_client_new ();
        fmq_client_configure (client, "client_test.cfg");
        fmq_client_setoption (client, "client/inbox", "./fmqroot/recv");
        fmq_client_connect   (client, "tcp://localhost:6000");
        fmq_client_subscribe (client, "/");
        fmq_client_subscribe (client, "/photos");
        fmq_client_subscribe (client, "/logs");
    }
    while (!zctx_interrupted)
        sleep (1);
    puts ("interrupted");

    fmq_server_destroy (&server);
    fmq_client_destroy (&client);
    return 0;
}
