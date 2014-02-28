/*
  TCPSocketServer.h
  2014 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:lawliet zou(lawliet.zou@gmail.com)
  2014-2-24

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include "Socket.h"
#include "TCPSocketConnection.h"

/** TCP Server.
  */
class TCPSocketServer : public Socket
{
public:
    /** Instantiate a TCP Server.
    */
    TCPSocketServer();

    /** Bind a socket to a specific port.
    \param port The port to listen for incoming connections on.
    \return 0 on success, -1 on failure.
    */
    int bind(int port);

    /** Start listening for incoming connections.
    \param backlog number of pending connections that can be queued up at any
                   one time [Default: 1].
    \return 0 on success, -1 on failure.
    */
    int listen(int backlog=1);

    /** Accept a new connection.
    \param connection A TCPSocketConnection instance that will handle the incoming connection.
    \return 0 on success, -1 on failure.
    */
    int accept(TCPSocketConnection& connection);
};

#endif
